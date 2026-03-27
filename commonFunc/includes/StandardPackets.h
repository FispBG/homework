//
// Created by fisp on 15.03.2026.
//
//
#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct Vec4 {
    uint32_t x, y, z, w;

    bool operator == (const Vec4 &toCheck) const {
        return x == toCheck.x && y == toCheck.y &&
               z == toCheck.z && w == toCheck.w;
    }
};
#pragma pack(pop)

enum class PacketType : uint8_t {
    STRING,
    VEC_FLOAT,
    VEC_INT
};
#pragma pack(push, 1)
struct HeaderString {
    PacketType type = PacketType::STRING;
    uint32_t size{};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PacketString {
    HeaderString header;
    char string[256]{};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PacketVector {
    PacketType type = PacketType::VEC_FLOAT;
    Vec4 data{};
};
#pragma pack(pop)
