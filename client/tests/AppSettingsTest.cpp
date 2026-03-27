//
// Created by fisp on 27.03.2026.
//

#include <gtest/gtest.h>
#include "../includes/AppSettings.h"
#include "../includes/NetWork.h"

TEST(AppSettingsTest, CreateConfigMissNeedFlags) {
    IpAddress ip;
    AppSettings settings(ip);

    const char *argv[] = {"./client", "-i", "1.1.2.3", "-p", "3443"};
    const ResultStatus res = settings.createConfig(4, argv);
    EXPECT_TRUE(res.isError());
}

TEST(AppSettingsTest, SetName) {
    IpAddress ip;
    AppSettings settings(ip);

    settings.setName("Vasya");
    EXPECT_EQ(settings.get().name, "Vasya");
}