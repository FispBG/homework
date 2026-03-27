//
// Created by fisp on 15.03.2026.
//

#pragma once

#include "../../commonFunc/includes/StandardPackets.h"
#include <string>
#include <cstring>

constexpr float matrixFloat[4][4] = {
    {1.0, 2.2, 3.6, 4},
    {5, 6, 7.9, 8},
    {9, 1, 2.9, 3},
    {4, 5.7, 6, 7}
};

constexpr int32_t matrixInt[4][4] = {
    {2, 3, 3, 5},
    {5, 6, 4, 8},
    {9, 1, 2, 8},
    {7, 5, 6, 7}
};

Vec4 textToIntVec(const std::string &inputText);

template <typename T>
Vec4 multiOnMatrix(const T x, const T y, const T z, const T w) {

    const T vec[4] = {x, y, z, w};
    T result[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::is_floating_point_v<T>) {
                result[i] += matrixFloat[i][j] * vec[j];
            }else if (std::is_integral_v<T>) {
                result[i] += matrixInt[i][j] * vec[j];
            }
        }
    }

    Vec4 toReturn{};

    memcpy(&toReturn.x, &result[0], sizeof(uint32_t));
    memcpy(&toReturn.y, &result[1], sizeof(uint32_t));
    memcpy(&toReturn.z, &result[2], sizeof(uint32_t));
    memcpy(&toReturn.w, &result[3], sizeof(uint32_t));

    return toReturn;
}
