//
// Created by fisp on 27.03.2026.
//

#include "../includes/ResultStatus.h"

#include <chrono>
#include <fstream>
#include <iomanip>

ResultStatus ResultStatus::Good() {
    ResultStatus good = {Status::Good, "", 0, ""};
    return good;
}

bool ResultStatus::isError() const {
    return condition == Status::Error;
}

bool ResultStatus::isWarning() const {
    return condition == Status::Warning;
}

bool ResultStatus::isGood() const {
    return condition == Status::Good;
}

bool ResultStatus::isNone() const {
    return condition == Status::None;
}

void logger(const ResultStatus &result) {
    if (result.isGood()) {
        return;
    }
    const std::string typeLog = result.isError() ? "[Error]" : "[Warning]";

    const auto systemTime = std::chrono::system_clock::now();
    const auto normalTime = std::chrono::system_clock::to_time_t(systemTime);

    const auto threadId = std::this_thread::get_id();

    std::ostringstream logString;

    logString << std::put_time(std::localtime(&normalTime), "%Y-%m-%d %H:%M:%S")
    << " | " << typeLog << " | " << "Thread: " << threadId << " | "
    << result.message << " | " << result.file << " | " << "line: " << result.line << std::endl;

    std::ofstream logFile("log.txt", std::ios_base::app);
    if (logFile.is_open()) {
        logFile << logString.str();
        logFile.close();
    }

    if (result.isError()) {
        std::cerr << result.message << std::endl;
    }else {
        std::cout << result.message << std::endl;
    }
}