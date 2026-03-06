//
// Created by fisp on 28.02.2026.
//

#ifndef HOMEWORK_APPSETTINGS_H
#define HOMEWORK_APPSETTINGS_H

#include "./functions.h"

// Инвариант - port находится в диапазоне [0, 65535]
// address - ipv4
// id > 0
// lib только существующая
// name не пустое
class AppSettings {
    std::string address;
    int32_t port = -1;
    std::string role = "Client";
    uint64_t id = -1;
    std::string lib;
    std::string name = "Guest";

    ResultStatus processOnlyIp(const std::string &ip);
    ResultStatus processOnlyPort(const std::string &port);
    ResultStatus processOnlyId(const std::string &id);
    ResultStatus processOnlyLib(std::string &lib, const std::string &argFlag);

    bool processNetworkFlags(uint64_t flag, const std::string &argFlag, ResultStatus &res);
    bool processUserFlags(uint64_t flag, const std::string &argFlag, ResultStatus &res);
    bool processAppFlags(uint64_t flag, const std::string &argFlag, ResultStatus &res);

    ResultStatus loopForIpnutArgs(const int &argc, const char *argv[]);
    ResultStatus processingFlag(uint64_t hashFlag, const std::string &flag, const std::string& argFlag);
    ResultStatus checkNeedFlag() const;

    public:

    ResultStatus createConfig(const int &argc, const char *argv[]);

    void setName(const std::string &newName);

    std::string getAddress() const;
    int32_t getPort() const;
    std::string getRole() const;
    uint64_t getId() const;

    std::string getLib() const;
    std::string getName() const;

};

#endif //HOMEWORK_APPSETTINGS_H