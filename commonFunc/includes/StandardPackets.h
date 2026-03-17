//
// Created by fisp on 15.03.2026.
//

#pragma once

#pragma pack(push, 1)
#include <stdint.h>
#include <string>

struct Vec4 {
    float x, y, z, w;

    bool operator == (const Vec4 &toCheck) const {
        return x == toCheck.x && y == toCheck.y &&
               z == toCheck.z && w == toCheck.w;
    }
};

enum class PacketType : uint8_t {
    STRING,
    VEC_FLOAT,
};

struct HeaderString {
    PacketType type = PacketType::STRING;
    uint32_t size;
};

struct PacketString {
    HeaderString header;
    char string[256];
};

struct PacketVector {
    PacketType type = PacketType::VEC_FLOAT;
    Vec4 data;
};

#pragma pack(pop)
