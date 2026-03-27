//
// Created by fisp on 27.03.2026.
//

#include <gtest/gtest.h>

#include "../includes/ConfigApp.h"

TEST(ConfigAppTest, NoInputArgsConfig) {
    ConfigApp app;
    const char* argv[] {"server"};
    const ResultStatus res = app.create(1, argv);
    EXPECT_TRUE(res.isError());
}

TEST(ConfigAppTest, NormalInputArgsConfig) {
    ConfigApp app;
    const char* argv[] {"server", "-p", "8080"};
    const ResultStatus res = app.create(3, argv);
    EXPECT_FALSE(res.isError());
    EXPECT_EQ(app.getConfig().port, 8080);
}

TEST(ConfigAppTest, InputInvalidPortConfig) {
    ConfigApp app;
    const char* argv[] {"server", "-p", "34534534"};
    const ResultStatus res = app.create(3, argv);
    EXPECT_TRUE(res.isError());
}

TEST(ConfigAppTest, InputInvalidFlag) {
    ConfigApp app;
    const char* argv[] {"server", "-i", "34534534"};
    const ResultStatus res = app.create(3, argv);
    EXPECT_TRUE(res.isError());
}