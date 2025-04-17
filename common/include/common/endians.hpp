#pragma once

#include "utils.hpp"

// don't need that actually at all
// but whatever...

#define FIELD(type, name) type name;

#define SWAP_FIELD(type, name) \
    if constexpr (std::is_same_v<type, uint16_t>) { name = swap_u16(name); } \
    else if constexpr (std::is_same_v<type, uint32_t>) { name = swap_u32(name); } \
    else if constexpr (std::is_same_v<type, int32_t>) { name = swap_i32(name); } \

#define DEFINE_SWAPPABLE_STRUCT(name, ...) \
struct name { \
    __VA_ARGS__ \
    void swap_endian();\
}; \
inline void name::swap_endian()
