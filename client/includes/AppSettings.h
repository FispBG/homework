//
// Created by fisp on 28.02.2026.
//

#pragma once

#include "./NetWork.h"
#include <string>
#include <stdint.h>

class AppSettings;

struct AppConfig {
    std::string role = "Client";
    uint64_t    id   = 0;
    std::string lib;
    std::string name = "Guest";
};

static ResultStatus processOnlyIp(const AppSettings *appThis, const std::string &ip);
static ResultStatus processOnlyPort(const AppSettings *appThis, const std::string &port);
static ResultStatus processOnlyId(AppSettings *appThis, std::string &id);
static ResultStatus processOnlyLib(AppSettings *appThis, std::string &lib, std::string &argFlag);

static bool processNetworkFlags(const AppSettings *appThis, uint64_t flag, std::string &argFlag);
static bool processUserFlags(AppSettings *appThis, uint64_t flag, std::string &argFlag);
static bool processAppFlags(AppSettings *appThis, uint64_t flag, std::string &argFlag);

// Инвариант - port находится в диапазоне [0, 65535]
// address - ipv4
// id > 0
// lib только существующая
// name не пустое

class AppSettings {
    IpAddress &network;
    AppConfig appConfig;

    friend ResultStatus processOnlyIp(const AppSettings *appThis, const std::string &ip);
    friend ResultStatus processOnlyPort(const AppSettings *appThis, const std::string &port);
    friend ResultStatus processOnlyId(AppSettings *appThis,  std::string &id);
    friend ResultStatus processOnlyLib(AppSettings *appThis, std::string &lib,  std::string &argFlag);

    friend bool processNetworkFlags(const AppSettings *appThis, uint64_t flag,  std::string &argFlag);
    friend bool processUserFlags(AppSettings *appThis, uint64_t flag,  std::string &argFlag);
    friend bool processAppFlags(AppSettings *appThis, uint64_t flag,  std::string &argFlag);

    ResultStatus processingFlag(uint64_t hashFlag, const std::string &flag, std::string &argFlag);
    ResultStatus checkNeedFlag() const;

    public:
    explicit AppSettings(IpAddress &settingNetwork) : network(settingNetwork) {};

    ResultStatus createConfig(int argc, const char *argv[]);

    void setName(const std::string &newName);
    void printHelp() const;
    const AppConfig& get() const;
};
