//
// Created by fisp on 26.03.2026.
//

#include "../includes/ConfigApp.h"
#include "../../commonFunc/includes/StringFunction.h"
#include "../../commonFunc/includes/VectorProcess.h"
#include "../../commonFunc/includes/ValidationFunction.h"

ResultStatus ConfigApp::processFlag(const uint64_t hashFlag, const std::string &flag, const std::string& argFlag) {
    ResultStatus res = ResultStatus::Good();

    const bool resultProcessing = processNetwork(hashFlag, argFlag);

    if (!resultProcessing) {
        return RES_ERROR("Invalid key: " + flag);
    }

    return res;
}

bool ConfigApp::processNetwork(const uint64_t hashFlag, const std::string &argFlag) {
    switch (hashFlag) {
        case hashString("-p"):
            if (!setPort(argFlag)) {
                logger(RES_ERROR("Invalid port: " + argFlag));
            }
            return true;

        default:
            return false;
    }
}

bool ConfigApp::setPort(const std::string &port) {
    if (isPort(port)) {
        config.port = stoi(port);
        config.setPort = true;
        return true;
    }

    return false;
}

ResultStatus ConfigApp::needFlag() const{
    const bool needFlag = config.setPort;

    if (needFlag) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Haven't need flags.");
}

Config ConfigApp::getConfig() const {
    return config;
}

ResultStatus ConfigApp::create(const int argc, const char *argv[]) {

    const ResultStatus statusLoop = loopForInputArgs(argc, argv,
        [this](const uint64_t hashFlag, const std::string &flag, const std::string& argFlag) {
            return this->processFlag(hashFlag, flag, argFlag);
        },
        [this] {
            return this->printHelp();
        });

    if (statusLoop.isError()) {
        return statusLoop;
    }

    const ResultStatus resCheckFlags = needFlag();
    logger(resCheckFlags);

    if (resCheckFlags.isError()) {
        return resCheckFlags;
    }

    return ResultStatus::Good();
}

void ConfigApp::printHelp() const{
    std::cout << R"(
        Need start command: ./server -p <port>
        Require:
              -p        Port to connect
        Example:
            ./server -p 8008
)";
}