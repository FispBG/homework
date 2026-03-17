//
// Created by fisp on 28.02.2026.
//

#include <gtest/gtest.h>
#include "../../commonFunc/includes/functions.h"
#include "../includes/Tests.h"
#include "../includes/Menu.h"
#include "../includes/DataPool.h"
#include "../includes/NetWork.h"

// fuctions.h

TEST(InputVectorTest, CheckNoNumberInString) {
    std::vector<int> vecInt(4);
    std::vector<float> vecFLoat(4);

    const ResultStatus res1 = fillVector("5 6 f 4", vecInt, vecInt.size());
    EXPECT_TRUE (res1.isError());

    const ResultStatus res2 = fillVector("5 7g76 56 4", vecInt, vecInt.size());
    EXPECT_TRUE (res2.isError());

    const ResultStatus res3 = fillVector("GDFG 6 56.7 4", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res3.isError());

    const ResultStatus res4 = fillVector("5 7g76 56.7 4", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res4.isError());
}

TEST(InputVectorTest, CheckWIsNotNull) {
    std::vector<int> vecInt(4);
    std::vector<float> vecFLoat(4);

    const ResultStatus res1 = fillVector("5 6 7 0", vecInt, vecInt.size());
    EXPECT_TRUE (res1.isError());

    const ResultStatus res2 = fillVector("5 45.8 56 0.0", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res2.isError());
}

TEST(FixStringTest, CheckDeleteNoKeyboardSymbol) {
    std::string str1 = "\nh\teLp";
    std::string fixString = fixInputString(str1);
    std::cout << fixString << std::endl;
    EXPECT_EQ(fixString, "help");
}

// datapool.h

TEST(DataPoolTest, InsertAndCheckEmpty) {
    DataPool<int, 4> pool;

    EXPECT_TRUE(pool.empty());

    pool.insert(10);
    EXPECT_FALSE(pool.empty());
}

TEST(DataPoolTest, FirstAndLastElement) {
    DataPool<int, 4> pool;

    pool.insert(88);
    pool.insert(5);
    pool.insert(37);

    EXPECT_EQ(pool.first(), 88);
    EXPECT_EQ(pool.last(),  37);
}

TEST(DataPoolTest, InsertOverThenSize) {
    DataPool<int, 2> pool;

    EXPECT_TRUE(pool.insert(1));
    EXPECT_TRUE(pool.insert(2));
    EXPECT_FALSE(pool.insert(3));
}

// appsettings.h

TEST(AppSettingsTest, CreateConfigMissNeedFlags) {
    AppSettings settings;

    const char *argv[] = {"-a 1.1.2.3 -p 3443"};
    const ResultStatus res = settings.createConfig(4, argv);
    EXPECT_TRUE(res.isError());
}

TEST(AppSettingsTest, SetName) {
    AppSettings settings;

    settings.setName("Vasya");
    EXPECT_EQ(settings.get().name, "Vasya");
}

// Tests.h

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