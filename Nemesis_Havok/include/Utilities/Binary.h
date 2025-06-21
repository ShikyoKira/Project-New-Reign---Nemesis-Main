#pragma once

#include <cstdint>

constexpr inline uint16_t BinaryReverse(uint16_t val)
{
    return (val >> 8) | (val << 8);
}

constexpr inline uint32_t BinaryReverse(uint32_t val)
{
    return (val >> 24) | ((val >> 8) & 0x0000FF00) | ((val << 8) & 0x00FF0000) | (val << 24);
}

constexpr inline uint64_t BinaryReverse(uint64_t val)
{
    return (val >> 56) | ((val >> 40) & 0x000000000000FF00ULL) | ((val >> 24) & 0x0000000000FF0000ULL)
           | ((val >> 8) & 0x00000000FF000000ULL) | ((val << 8) & 0x000000FF00000000ULL)
           | ((val << 24) & 0x0000FF0000000000ULL) | ((val << 40) & 0x00FF000000000000ULL) | (val << 56);
}
