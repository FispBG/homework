//
// Created by fisp on 27.03.2026.
//

#include "../includes/VectorProcess.h"

ResultStatus fillVectors(std::vector<std::string> &stringVec,
    std::vector<int64_t> &intVec,
    std::vector<float> &floatVec,
    const std::string &type,
    const std::string &str,
    const uint64_t vecSize) {

    const uint64_t hashType = hashString(type.c_str());

    switch (hashType) {
        case hashString("int"):
            return fillVector(str, intVec, vecSize);

        case hashString("float"):
            return fillVector(str, floatVec, vecSize);

        case hashString("string"):
            return fillVector(str, stringVec, vecSize);

        default:
            return RES_ERROR("Unknown hash type.");
    }
}

ResultStatus processInputVector(const std::string &type,
                      std::vector<std::string> &stringVec,
                      std::vector<int64_t> &intVec,
                      std::vector<float> &floatVec, const uint64_t size)  {

    std::string inputStr;
    std::cout << "Input " << type << " vector 4 elem separated by space (" << type << ' ' << type << ' ' << type << ' ' << type << ") : ";
    std::getline(std::cin, inputStr);
    const ResultStatus res = fillVectors(stringVec, intVec, floatVec, type, inputStr, size);

    return res;
}