//
// Created by fisp on 26.03.2026.
//

#pragma once

#include "../../commonFunc/includes/ResultStatus.h"

struct Config {
    uint16_t port = 8080;
    bool setPort = false;
};

class ConfigApp {
    Config config{};

    ResultStatus processFlag(uint64_t hashFlag, const std::string &flag, const std::string& argFlag);
    bool processNetwork(uint64_t hashFlag, const std::string &argFlag);
    ResultStatus needFlag() const;
    bool setPort(const std::string &port);
public:
    ResultStatus create(int argc, const char *argv[]);
    Config getConfig() const;
    void printHelp() const;
};