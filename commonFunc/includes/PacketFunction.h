//
// Created by fisp on 27.03.2026.
//

#pragma once

#include "./StandardPackets.h"
#include <string>
#include <cstring>
#include <netinet/in.h>

PacketString createPacketString(const std::string &str);

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
PacketVector createPacketVector(T x, T y, T z, T w) {
    PacketVector packet{};
    packet.type = PacketType::VEC_FLOAT;

    auto toNetwork = [](T value) {
        uint32_t bytes;
        std::memcpy(&bytes, &value, sizeof(uint32_t));
        return htonl(bytes);
    };

    packet.data.x = toNetwork(x);
    packet.data.y = toNetwork(y);
    packet.data.z = toNetwork(z);
    packet.data.w = toNetwork(w);

    return packet;
}

template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
PacketVector createPacketVector(T x, T y, T z, T w) {
    PacketVector packet{};
    packet.type = PacketType::VEC_INT;

    auto toNetwork = [](T value) {
        uint32_t bytes;
        std::memcpy(&bytes, &value, sizeof(uint32_t));
        return htonl(bytes);
    };

    packet.data.x = toNetwork(x);
    packet.data.y = toNetwork(y);
    packet.data.z = toNetwork(z);
    packet.data.w = toNetwork(w);

    return packet;
}