//
// Created by fisp on 27.03.2026.
//

#include "../includes/DataPool.h"

std::vector<std::string>& MyVec::getStringVec() {
    return stringVec;
}

std::vector<float>& MyVec::getFloatVec() {
    return floatVec;
}

std::vector<int64_t>& MyVec::getIntVec() {
    return intVec;
}

std::string MyVec::getType() const {
    return type;
}
