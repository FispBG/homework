//
// Created by fisp on 28.02.2026.
//
#include "../includes/AppSettings.h"
#include "../../commonFunc/includes/StringFunction.h"
#include "../../commonFunc/includes/VectorProcess.h"
#include <filesystem>

void AppSettings::setName(const std::string &newName) {
    appConfig.name = newName;
}

static ResultStatus processOnlyIp(const AppSettings *appThis, const std::string &ip) {
    if (appThis -> network.setIp(ip)) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Invalid address: " + ip);
}

static ResultStatus processOnlyPort(const AppSettings *appThis, const std::string &inputPort) {
    if (appThis -> network.setPort(inputPort)) {
        return ResultStatus::Good();
    }

    return RES_ERROR("Invalid port(0-65535): " + inputPort);
}

static ResultStatus processOnlyId(AppSettings *appThis, std::string &inputId) {
    char* firstNotInt = nullptr;
    const uint64_t valueId = strtoll(inputId.c_str(), &firstNotInt, 10);

    if (*firstNotInt != '\0') {
        return RES_ERROR("Invalid id: " + inputId);
    }

    appThis -> appConfig.id = valueId;
    return ResultStatus::Good();
}

/**
 * \brief Обрабатывает сетевые флаги.
 *
 * Поддерживаемые флаги:
 *   -i - устанавливает и валидирует IP-адрес.
 *   -p - устанавливает и валидирует номер порта.
 *
 * \param appThis  Указатель на экземпляр класса AppSettings.
 * \param flag     Заранее вычисленный хэш строки флага.
 * \param argFlag  Значение аргумента, связанного с флагом.
 * \return true, при распознании флага, иначе false.
 */
static bool processNetworkFlags(const AppSettings *appThis, const uint64_t flag, std::string &argFlag) {

    switch (flag) {
        case hashString("-i"):
            if (!processOnlyIp(appThis, argFlag).isGood()) {
                logger(RES_ERROR("Invalid IP address: " + argFlag));
            }
            return true;

        case hashString("-p"):
            if (!processOnlyPort(appThis, argFlag).isGood()) {
                logger(RES_ERROR("Invalid Port: " + argFlag));
            }
            return true;

        default:
            return false;
    }
}

/**
 * \brief Обрабатывает пользовательские флаги.
 *
 * Поддерживаемые флаги:
 *   -r - устанавливает роль пользователя.
 *   -id - устанавливает и валидирует id пользователя.
 *
 * \param appThis  Указатель на экземпляр класса AppSettings.
 * \param flag     Заранее вычисленный хэш строки флага.
 * \param argFlag  Значение аргумента, связанного с флагом.
 * \return true, при распознании флага, иначе false.
 */
static bool processUserFlags(AppSettings *appThis, const uint64_t flag, std::string &argFlag) {

    switch (flag) {
        case hashString("-r"):
            appThis -> appConfig.role = argFlag;
            return true;

        case hashString("-id"):
            if (!processOnlyId(appThis, argFlag).isGood()) {
                logger(RES_ERROR("Invalid ID: " + argFlag));
            }
            return true;

        default:
            return false;
    }
}

static ResultStatus processOnlyLib(AppSettings *appThis, std::string &lib, std::string &argFlag) {
    std::filesystem::path libPath = std::filesystem::path(argFlag);

    if (std::filesystem::exists(libPath)) {
        lib = libPath;
        return ResultStatus::Good();
    }

    return RES_ERROR("Lib not exists: " + argFlag);
}


/**
 * \brief Обрабатывает приложенческие флаги.
 *
 * Поддерживаемые флаги:
 *   -L - устанавливает и валидирует путь к переданной директории.
 *
 * \param appThis  Указатель на экземпляр класса AppSettings.
 * \param flag     Заранее вычисленный хэш строки флага.
 * \param argFlag  Значение аргумента, связанного с флагом.
 * \return true, при распознании флага, иначе false.
 */
static bool processAppFlags(AppSettings *appThis, const uint64_t flag, std::string &argFlag) {

    switch (flag) {
        case hashString("-L"):
            if (!processOnlyLib(appThis, appThis -> appConfig.lib, argFlag).isGood()) {
                logger(RES_ERROR("Invalid lib: " + appThis -> appConfig.lib));
            }
            return true;

        default:
            return false;
    }
}

/**
 * \brief Обрабатывает флаг командной строки и передаёт его в соответствующий обработчик.
 *
 * Пытается найти и обработать флаг в сетевых, пользовательских
 * и приложенческих обработчиках. Возвращает ошибку, если ни один обработчик не распознал флаг.
 *
 * \param hashFlag  Заранее вычисленный хэш строки флага.
 * \param flag      Строка с флагом, нужен при выводе ошибки.
 * \param argFlag   Значение аргумента, связанного с флагом.
 * \return ResultStatus::Good() при распознании флага, иначе RES_ERROR("Invalid key: " + flag).
 */
ResultStatus AppSettings::processingFlag(const uint64_t hashFlag, const std::string &flag, std::string& argFlag) {

    ResultStatus res = ResultStatus::Good();

    const bool resultProcessing = processNetworkFlags(this, hashFlag, argFlag) ||
                            processUserFlags(this, hashFlag, argFlag) ||
                            processAppFlags(this, hashFlag, argFlag);

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

ResultStatus AppSettings::createConfig(const int argc, const char *argv[]) {

    const ResultStatus statusLoop = loopForInputArgs(argc, argv,
        [this](const uint64_t hashFlag, const std::string &flag, std::string& argFlag) {
            return this->processingFlag(hashFlag, flag, argFlag);
        },
        [this] {
            return this->printHelp();
        });

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

void AppSettings::printHelp() const{
    std::cout << R"(
        Need start command: ./client -i <ip> -p <port> -i <id> [-r <role>] [-l <lib>]
        Require:
              -i        IP to connect
              -p        Port to connect
        Optional arguments:
              -r        Role, default set Client
              -L        Library default
              -id        Client id
              --help    Show help doc
        Example:
            ./client -i 127.0.0.1 -p 8008
)";
}