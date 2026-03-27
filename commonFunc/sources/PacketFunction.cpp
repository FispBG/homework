//
// Created by fisp on 27.03.2026.
//

#include "../includes/PacketFunction.h"

PacketString createPacketString(const std::string &str) {
    PacketString packet{};

    const uint32_t len = std::min(static_cast<uint32_t>(str.length()),static_cast<uint32_t> (255));
    packet.header.size = htonl(len);

    std::memcpy(packet.string, str.c_str(), len);
    packet.string[len] = '\0';

    return packet;
}