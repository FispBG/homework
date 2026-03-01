//
// Created by fisp on 28.02.2026.
//
#include "../includes/AppSettings.h"

#include <filesystem>

void AppSettings::setName(const std::string &newName) {
    name = newName;
}

std::string AppSettings::getAddress() const {
    return address;
}

int32_t AppSettings::getPort() const {
    return port;
}

uint64_t AppSettings::getId() const {
    return id;
}

std::string AppSettings::getRole() const {
    return role;
}

std::string AppSettings::getLib() const {
    return lib;
}

std::string AppSettings::getName() const {
    return name;
}

ResultStatus AppSettings::processOnlyIp(const std::string &ip) {
    if (isIpAddress(ip)) {
         address = ip;
        return ResultStatus::Good();
    }

    return ResultStatus::Error("Invalid address: " + ip);
}

ResultStatus AppSettings::processOnlyPort(const std::string &inputPort) {
    if (isPort(inputPort)) {
        port = strtol(inputPort.c_str(), nullptr, 10);
        return ResultStatus::Good();
    }

    return ResultStatus::Error("Invalid port(0-65535): " + inputPort);
}

ResultStatus AppSettings::processOnlyId(const std::string &inputId) {
    char* firstNotInt = nullptr;
    const uint64_t valueId = strtol(inputId.c_str(), &firstNotInt, 10);

    if (*firstNotInt != '\0') {
        return ResultStatus::Error("Invalid id: " + inputId);
    }

    id = valueId;
    return ResultStatus::Good();
}

bool AppSettings::processNetworkFlags(const uint64_t flag, const std::string &argFlag, ResultStatus &res) {

    switch (flag) {
        case hashString("-a"):
            res = processOnlyIp(argFlag);
            return true;

        case hashString("-p"):
            res = processOnlyPort(argFlag);
            return true;

        default:
            return false;
    }
}

bool AppSettings::processUserFlags(const uint64_t flag, const std::string &argFlag, ResultStatus &res) {

    switch (flag) {
        case hashString("-r"):
            role = argFlag;
            res = ResultStatus::Good();
            return true;

        case hashString("-i"):
            res = processOnlyId(argFlag);
            return true;

        default:
            return false;
    }
}

ResultStatus AppSettings::processOnlyLib(std::string &lib, const std::string &argFlag) {
    std::filesystem::path libPath = std::filesystem::path(argFlag);

    if (std::filesystem::exists(libPath)) {
        lib = libPath;
        return ResultStatus::Good();
    }

    return ResultStatus::Error("Lib not exists: " + argFlag);
}

bool AppSettings::processAppFlags(const uint64_t flag, const std::string &argFlag, ResultStatus &res) {

    switch (flag) {
        case hashString("-L"):
            res = processOnlyLib(lib, argFlag);
            return true;

        default:
            return false;
    }
}

ResultStatus AppSettings::processingFlag(const uint64_t hashFlag, const std::string &flag, const std::string& argFlag) {

    ResultStatus res = ResultStatus::Good();

    const bool resultProcessing = processNetworkFlags(hashFlag, argFlag, res) ||
                            processUserFlags(hashFlag, argFlag, res) ||
                            processAppFlags(hashFlag, argFlag, res);

    if (!resultProcessing) {
        return ResultStatus::Error("Invalid key: " + flag);
    }

    return res;
}

ResultStatus AppSettings::checkNeedFlag() const {
    const bool needFlag = !address.empty() &&
        port != -1 &&
        id != -1;

    if (needFlag) {
        return ResultStatus::Good();
    }

    return ResultStatus::Error("Haven't need flags.");
}

ResultStatus AppSettings::createConfig(const int &argc, const char *argv[]) {

    for (int i = 1; i < argc; i++) {
        const uint64_t hashFlag = hashString(argv[i]);
        const std::string flag = argv[i];

        if (i+1 >= argc) {
            return ResultStatus::Error("No argument for flag: " + flag);
        }

        const std::string argFlag = argv[++i];

        if (argFlag[0] == '-') {
            return ResultStatus::Error("Invalid arg flag: " + argFlag);
        }

        const ResultStatus res = processingFlag(hashFlag, flag, argFlag);
        logger(res);

        if (res.isError()) {
            return res;
        }
    }

    const ResultStatus res = checkNeedFlag();
    logger(res);

    if (res.isError()) {
        return res;
    }

    return ResultStatus::Good();
}