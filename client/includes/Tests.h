//
// Created by fisp on 02.03.2026.
//

#pragma once

#include <vector>
#include <string>

class Test {
public:
    virtual ~Test() = default;
    virtual bool test() = 0;
};
// Инвариант - params не пустой
class ConnectionTest : public Test {
    std::vector<std::string> params;
public:
    bool test() override;
    explicit ConnectionTest(const std::vector<std::string> &inputParams) : params(inputParams) {};
};

// Инвариант - params не пустой
// Желательно проверить lib на существование
class ResourceTest : public Test {
    std::vector<std::string> params;
    std::string lib;

public:
    bool test() override;
    explicit ResourceTest(const std::vector<std::string> &inputParams,
        const std::string &baseLib) : params(inputParams), lib(baseLib){};
};
