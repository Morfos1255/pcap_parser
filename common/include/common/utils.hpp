#pragma once

#include <cstdint>

inline uint16_t swap_u16(uint16_t val) {
  return (val << 8) | (val >> 8);
}

inline uint32_t swap_u32(uint32_t val) {
  return  ((val >> 24) & 0xff) |
          ((val >> 8) & 0xff00) |
          ((val << 8) & 0xff0000) |
          ((val << 24) & 0xff000000);
}

inline int32_t swap_i32(int32_t val) {
  return static_cast<int32_t>(swap_u32(static_cast<uint32_t>(val)));
}
