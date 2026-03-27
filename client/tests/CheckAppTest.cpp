//
// Created by fisp on 27.03.2026.
//

#include <gtest/gtest.h>
#include "../includes/Tests.h"

TEST(ConnectionTestTest, TestWrongAddress) {
    const std::vector<std::string> params = {"256.256.256.256:9999"};
    ConnectionTest connect(params);

    EXPECT_FALSE(connect.test());
}

TEST(ResourceTestTest, NonExistentLibReturnsFalse) {
    const std::vector<std::string> params = {"sdfsdf.json"};
    ResourceTest resource(params, "./");

    EXPECT_FALSE(resource.test());
}