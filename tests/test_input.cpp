//
// Created by fisp on 28.02.2026.
//

#include <gtest/gtest.h>
#include "../includes/functions.h"

TEST(InputVectorTest, CheckNoNumberInString) {
    std::vector<int> vecInt(4);
    std::vector<float> vecFLoat(4);

    const ResultStatus res1 = fillIntVector("5 6 f 4", vecInt, vecInt.size());
    EXPECT_TRUE (res1.isError());

    const ResultStatus res2 = fillIntVector("5 7g76 56 4", vecInt, vecInt.size());
    EXPECT_TRUE (res2.isError());

    const ResultStatus res3 = fillFloatVector("GDFG 6 56.7 4", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res3.isError());

    const ResultStatus res4 = fillFloatVector("5 7g76 56.7 4", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res4.isError());
}

TEST(InputVectorTest, CheckWIsNotNull) {
    std::vector<int> vecInt(4);
    std::vector<float> vecFLoat(4);

    const ResultStatus res1 = fillIntVector("5 6 7 0", vecInt, vecInt.size());
    EXPECT_TRUE (res1.isError());

    const ResultStatus res2 = fillFloatVector("5 45.8 56 0.0", vecFLoat, vecFLoat.size());
    EXPECT_TRUE (res2.isError());
}

TEST(FixStringTest, CheckDeleteNoKeyboardSimbol) {
    std::string str1 = "\nh\teLp";
    std::string fixString = fixInputString(str1);
    std::cout << fixString << std::endl;
    EXPECT_EQ(fixString, "help");
}