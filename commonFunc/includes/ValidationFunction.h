//
// Created by fisp on 27.03.2026.
//

#pragma once

#include "./ResultStatus.h"

bool isIpAddress(const std::string &ip);
bool isPort(const std::string &port);

void processInputType(std::string &type);
ResultStatus processInputName(std::string &name);