//
// Created by fisp on 02.03.2026.
//

#ifndef HOMEWORK_TESTS_H
#define HOMEWORK_TESTS_H
#include <vector>
#include <string>

class Test {
public:
    virtual ~Test() = default;
    virtual void test() = 0;
};

class ConnectionTest : public Test {
    std::vector<std::string> params;
public:
    void test() override;
    explicit ConnectionTest(const std::vector<std::string> &inputParams) : params(inputParams) {};
};

class ResourceTest : public Test {
    std::vector<std::string> params;
    std::string lib;

public:
    void test() override;
    explicit ResourceTest(const std::vector<std::string> &inputParams,
        const std::string &baseLib) : params(inputParams), lib(baseLib){};
};

#endif //HOMEWORK_TESTS_H