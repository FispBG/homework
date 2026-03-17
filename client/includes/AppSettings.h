//
// Created by fisp on 28.02.2026.
//

#pragma once

#include "../../commonFunc/includes/functions.h"
#include <string>
#include <stdint.h>

struct AppConfig {
    std::string address;
    int32_t     port = -1;
    std::string role = "Client";
    uint64_t    id   = 0;
    std::string lib;
    std::string name = "Guest";
};

// Инвариант - port находится в диапазоне [0, 65535]
// address - ipv4
// id > 0
// lib только существующая
// name не пустое
class AppSettings {
    AppConfig appConfig;

    ResultStatus processOnlyIp(const std::string &ip);
    ResultStatus processOnlyPort(const std::string &port);
    ResultStatus processOnlyId(const std::string &id);
    ResultStatus processOnlyLib(std::string &lib, const std::string &argFlag);

    bool processNetworkFlags(uint64_t flag, const std::string &argFlag);
    bool processUserFlags(uint64_t flag, const std::string &argFlag);
    bool processAppFlags(uint64_t flag, const std::string &argFlag);

    ResultStatus loopForIpnutArgs(int argc, const char *argv[]);
    ResultStatus processingFlag(uint64_t hashFlag, const std::string &flag, const std::string& argFlag);
    ResultStatus checkNeedFlag() const;

    public:

    ResultStatus createConfig(int argc, const char *argv[]);

    void setName(const std::string &newName);

    const AppConfig& get() const;
};
