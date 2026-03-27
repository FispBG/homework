//
// Created by fisp on 26.03.2026.
//
#include <gtest/gtest.h>

#include "../../commonFunc/includes/StandardPackets.h"
#include "../includes/MathFunc.h"

TEST(MultiOnMatrixTest, FloatResultNotZero) {
    const Vec4 result = multiOnMatrix(1.0f, 2.0f, 3.0f, 4.0f);

    const Vec4 zero  = {0, 0, 0, 0};
    EXPECT_FALSE(result == zero);
}

TEST(MultiOnMatrixTest, IntResultNotZero) {
    const Vec4 result = multiOnMatrix(1, 2, 3, 4);
    const Vec4 zero   = {};
    EXPECT_FALSE(result == zero);
}

TEST(MultiOnMatrixTest, ZeroInputIsZero) {
    const Vec4 result = textToIntVec("dsf f dsf hh");
    const Vec4 zero   = {3, 1, 3, 2};
    EXPECT_TRUE(result == zero);
}