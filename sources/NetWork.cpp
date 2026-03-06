//
// Created by fisp on 04.03.2026.
//

#include "../includes/NetWork.h"
#include "../includes/functions.h"

ResultStatus IpAddress::setIpAddress(const std::string &ipStr){
    ResultStatus status = ResultStatus::Good();

    if (!isIpAddress(ipStr)) {
        status = ResultStatus::Error("Input not valid ip (need 10.10.10.10 format).");
        return status;
    }

    const std::vector<std::string> bytes = split(ipStr, '.');

    for (uint8_t i = 0; i < bytes.size(); i++) {
        ip[i] = strtol(bytes[i].c_str(), nullptr, 10);
    }

    ipInput = true;
    return status;
}

ResultStatus IpAddress::setIpAddress(const std::vector<int32_t> &ipVectorInt) {
    ResultStatus status = ResultStatus::Good();

    if (ipVectorInt.size() != 4) {
        status = ResultStatus::Error("Size vector != 4.");
        return status;
    }

    for (uint8_t i = 0; i < ipVectorInt.size(); i++) {
        if (!(ipVectorInt[i] >= 0 && ipVectorInt[i] <= 255)) {
            status = ResultStatus::Error(
                "One element in vector not in 0 <= byte <= 255. is: " +
                std::to_string(ipVectorInt[i])
                );
            return status;
        }
        ip[i] = ipVectorInt[i];
    }

    ipInput = true;
    return status;
}

ResultStatus IpAddress::setIpAddress(const uint32_t hex) {
    for (uint8_t i = 0; i < 4; i++) {
        const uint32_t shiftInt = hex >> (24 - 8 * i);
        ip[i] = shiftInt & 0xff;
    }

    ipInput = true;
    return ResultStatus::Good();
}

std::ostream& operator << (std::ostream& os, const IpAddress &netAddress) {

    if (!netAddress.ipInput) {
        return os << "Ip not input." << std::endl;
    }
    for (uint8_t i = 0; i < 4; i++) {
        os << netAddress.ip[i];
        if (i < 3) {
            os << ".";
        }
    }
    return os;
}
