//
// Created by fisp on 27.03.2026.
//

#pragma once

#include <string>
#include <thread>
#include <iostream>

enum class Status {
    Good,
    Error,
    Warning,
    None
};

struct ResultStatus {
    Status condition = Status::None;
    std::string message;
    int64_t line = 0;
    const char* file = "";

    static ResultStatus Good();

    bool isWarning() const;
    bool isError() const;
    bool isGood() const;
    bool isNone() const;
};

#define RES_ERROR(msg) ResultStatus{Status::Error, msg, __LINE__, __FILE__}
#define RES_WARNING(msg) ResultStatus{Status::Warning, msg, __LINE__, __FILE__}

void logger(const ResultStatus &result);