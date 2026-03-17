//
// Created by fisp on 15.03.2026.
//

#include "../includes/MathFunc.h"

Vec4 multiOnMatrix(const Vec4 inputVec) {

    constexpr float matrix[4][4] = {
        {1, 2.2, 3, 4},
        {5, 6, 7.9, 8},
        {9, 1, 2, 3},
        {4, 5.7, 6, 7}
    };

    const float vec[4] = {inputVec.x, inputVec.y, inputVec.z, inputVec.w};
    float result[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    return Vec4{result[0], result[1], result[2], result[3]};
}