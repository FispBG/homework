//
// Created by fisp on 04.03.2026.
//

#include "../includes/NetWork.h"
#include "../../commonFunc/includes/functions.h"

ResultStatus IpAddress::setIpAddress(const std::string &ipStr){
    ResultStatus status = ResultStatus::Error("Input not valid ip (need 10.10.10.10:5435 format).");

    const std::vector<std::string> ipAndPort = split(ipStr, ':');

    if (ipAndPort.size() != 2) {
        return status;
    }

    if (!(isIpAddress(ipAndPort[0]) && isPort(ipAndPort[1]))) {
        return status;
    }

    const std::vector<std::string> bytes = split(ipAndPort[0], '.');

    for (uint8_t i = 0; i < bytes.size(); i++) {
        ip[i] = strtol(bytes[i].c_str(), nullptr, 10);
    }
    port = strtol(ipAndPort[1].c_str(), nullptr, 10);

    status = ResultStatus::Good();
    ipInput = true;
    return status;
}

ResultStatus IpAddress::setIpAddress(const std::vector<int64_t> &ipVectorInt) {
    ResultStatus status = ResultStatus::Good();

    if (ipVectorInt.size() != 5) {
        status = ResultStatus::Error("Size vector != 5.");
        return status;
    }

    for (uint8_t i = 0; i < ipVectorInt.size() - 1; i++) {
        if (!(ipVectorInt[i] >= 0 && ipVectorInt[i] <= 255)) {
            status = ResultStatus::Error(
                "One element in vector not in 0 <= byte <= 255. is: " +
                std::to_string(ipVectorInt[i])
                );
            return status;
        }
        ip[i] = ipVectorInt[i];
    }

    if (ipVectorInt[4] < 0 || ipVectorInt[4] > 65535) {
        status = ResultStatus::Error("Port invalid.");
        return status;
    }
    port = ipVectorInt[4];

    ipInput = true;
    return status;
}

ResultStatus IpAddress::setIpAddress(const uint64_t hex) {
    port = hex & 0xffff;

    const uint32_t ipVal = (hex >> 16) & 0xffffffff;

    for (uint8_t i = 0; i < 4; i++) {
        const uint64_t shiftInt = ipVal >> (24 - 8 * i);
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

    os << ":" << netAddress.port;
    return os;
}

std::string IpAddress::getAddress() const {
    return std::to_string(ip[0]) + "." +
           std::to_string(ip[1]) + "." +
           std::to_string(ip[2]) + "." +
           std::to_string(ip[3]) + ":" +
           std::to_string(port);
}

bool IpAddress::checkIpInput() const {
    return ipInput;
}

uint16_t IpAddress::getPort() const {
    return port;
}

std::string IpAddress::getIp() const {
    return std::to_string(ip[0]) + "." + std::to_string(ip[1]) + "." +
           std::to_string(ip[2]) + "." + std::to_string(ip[3]);
}