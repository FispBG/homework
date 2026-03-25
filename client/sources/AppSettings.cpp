//
// Created by fisp on 28.02.2026.
//
#include "../includes/AppSettings.h"

#include <filesystem>

void AppSettings::setName(const std::string &newName) {
    appConfig.name = newName;
}

ResultStatus AppSettings::processOnlyIp(const std::string &ip) {
    if (network.setIp(ip)) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Invalid address: " + ip);
}

ResultStatus AppSettings::processOnlyPort(const std::string &inputPort) {
    if (network.setPort(inputPort)) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Invalid port(0-65535): " + inputPort);
}

ResultStatus AppSettings::processOnlyId(const std::string &inputId) {
    char* firstNotInt = nullptr;
    const uint64_t valueId = strtol(inputId.c_str(), &firstNotInt, 10);

    if (*firstNotInt != '\0') {
        return RES_ERROR("Invalid id: " + inputId);
    }

    appConfig.id = valueId;
    return ResultStatus::Good();
}

bool AppSettings::processNetworkFlags(const uint64_t flag, const std::string &argFlag) {

    switch (flag) {
        case hashString("-a"):
            if (!processOnlyIp(argFlag).isGood()) {
                logger(RES_ERROR("Invalid IP address: " + argFlag));
            }
            return true;

        case hashString("-p"):
            if (!processOnlyPort(argFlag).isGood()) {
                logger(RES_ERROR("Invalid Port: " + argFlag));
            }
            return true;

        default:
            return false;
    }
}

bool AppSettings::processUserFlags(const uint64_t flag, const std::string &argFlag) {

    switch (flag) {
        case hashString("-r"):
            appConfig.role = argFlag;
            return true;

        case hashString("-i"):
            if (!processOnlyId(argFlag).isGood()) {
                logger(RES_ERROR("Invalid ID: " + argFlag));
            }
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

    return RES_ERROR("Lib not exists: " + argFlag);
}

bool AppSettings::processAppFlags(const uint64_t flag, const std::string &argFlag) {

    switch (flag) {
        case hashString("-L"):
            if (!processOnlyLib(appConfig.lib, argFlag).isGood()) {
                logger(RES_ERROR("Invalid lib: " + appConfig.lib));
            }
            return true;

        default:
            return false;
    }
}

ResultStatus AppSettings::processingFlag(const uint64_t hashFlag, const std::string &flag, const std::string& argFlag) {

    ResultStatus res = ResultStatus::Good();

    const bool resultProcessing = processNetworkFlags(hashFlag, argFlag) ||
                            processUserFlags(hashFlag, argFlag) ||
                            processAppFlags(hashFlag, argFlag);

    if (!resultProcessing) {
        return RES_ERROR("Invalid key: " + flag);
    }

    return res;
}

ResultStatus AppSettings::checkNeedFlag() const {
    const bool needFlag = network.checkIpInput() &&
        network.checkPortInput();

    if (needFlag) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Haven't need flags.");
}

ResultStatus AppSettings::loopForInputArgs(const int argc, const char *argv[]) {
    for (int i = 1; i < argc; i++) {
        const uint64_t hashFlag = hashString(argv[i]);
        const std::string flag = argv[i];

        if (i+1 >= argc) {
            return RES_ERROR("No argument for flag: " + flag);
        }

        const std::string argFlag = argv[++i];

        if (argFlag[0] == '-') {
            return RES_ERROR("Invalid arg flag: " + argFlag);
        }

        const ResultStatus res = processingFlag(hashFlag, flag, argFlag);
        logger(res);

        if (res.isError()) {
            return res;
        }
    }

    return ResultStatus::Good();
}

ResultStatus AppSettings::createConfig(const int argc, const char *argv[]) {

    const ResultStatus statusLoop = loopForInputArgs(argc, argv);

    if (statusLoop.isError()) {
        return statusLoop;
    }

    const ResultStatus resCheckFlags = checkNeedFlag();
    logger(resCheckFlags);

    if (resCheckFlags.isError()) {
        return resCheckFlags;
    }

    return ResultStatus::Good();
}

const AppConfig &AppSettings::get() const {
    return appConfig;
}