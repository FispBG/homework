//
// Created by fisp on 15.03.2026.
//

#include "../includes/MathFunc.h"
#include "../../commonFunc/includes/StringFunction.h"

Vec4 textToIntVec(const std::string &inputText) {
    const std::vector<std::string> splitText = split(inputText, ' ');

    if (splitText.size() < 4) {
        return Vec4();
    }

    Vec4 intVec;

    intVec.x = static_cast<uint32_t>(splitText[0].length());
    intVec.y = static_cast<uint32_t>(splitText[1].length());
    intVec.z = static_cast<uint32_t>(splitText[2].length());
    intVec.w = static_cast<uint32_t>(splitText[3].length());

    return intVec;
}