//
// Created by fisp on 27.03.2026.
//

#include <gtest/gtest.h>
#include "../includes/DataPool.h"

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