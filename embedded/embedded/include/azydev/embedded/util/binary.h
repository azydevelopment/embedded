/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

// TODO HACK: Remove all references to double underscores or identifiers
// starting with an underscore followed by an uppercase letter

#include <stdint.h>

// binary related types

template <typename POINTER_TYPE> struct BIT_POINTER {
  volatile POINTER_TYPE *address;
  uint8_t offset;
};

template <typename POINTER_TYPE> struct BIT_RANGE {
  volatile POINTER_TYPE *address;
  uint8_t lsb_offset;
  uint8_t length;
};

class Binary final {
public:
  // create one hot
  template <typename PRIMITIVE> static inline PRIMITIVE OH(uint8_t bit) {
    return PRIMITIVE(1) << bit;
  };

  // create bit mask
  template <typename PRIMITIVE>
  static inline PRIMITIVE BM(uint8_t offset, uint8_t length) {
    return ((PRIMITIVE(1) << length) - 1) << offset;
  };

  // create masked value
  template <typename PRIMITIVE>
  static inline PRIMITIVE BM(PRIMITIVE valueToMask, uint8_t offset,
                             uint8_t length) {
    return valueToMask & BM<PRIMITIVE>(offset, length);
  };

  // create bit range
  template <typename PRIMITIVE>
  static inline PRIMITIVE BR(uint8_t lsbOffset, uint8_t length,
                             PRIMITIVE value) {
    return ((value << lsbOffset) & BM<PRIMITIVE>(lsbOffset, length));
  };

  // bit check
  template <typename PRIMITIVE>
  static inline PRIMITIVE BC(PRIMITIVE target, uint8_t bit) {
    return (target & OH<PRIMITIVE>(bit)) >> (bit);
  };

  // bit check range
  template <typename PRIMITIVE>
  static inline PRIMITIVE BCR(PRIMITIVE target, uint8_t lsbOffset,
                              uint8_t length) {
    return (target & (BM<PRIMITIVE>(lsbOffset, length))) >> lsbOffset;
  };

  // bit set
  template <typename PRIMITIVE>
  static inline void BS(PRIMITIVE &target, uint8_t bit, bool setHigh) {
    setHigh ? (target |= (PRIMITIVE(1) << (bit)))
            : (target &= ~(PRIMITIVE(1) << (bit)));
  };

  // bit set range
  template <typename PRIMITIVE>
  static inline void BSR(PRIMITIVE &target, uint8_t lsbOffset, uint8_t length,
                         PRIMITIVE value) {
    target = (target & ~BM<PRIMITIVE>(lsbOffset, length)) |
             BR(lsbOffset, length, value);
  };

  template <typename PRIMITIVE>
  static inline bool IsParityEven(PRIMITIVE data) {
    uint8_t parityEven = 0;
    for (uint8_t i = 0; i < (sizeof(data) * 8); i++) {
      parityEven ^= BC(data, i);
    }
    return parityEven != 0;
  };

  template <typename PRIMITIVE> static inline bool IsParityOdd(PRIMITIVE data) {
    return !IsParityEven(data);
  };
};

/* Binary constant generator macro
By Tom Torfs - donated to the public domain
*/

/* All macro's evaluate to compile-time constants */

/* *** helper macros *** /

/* turn a numeric literal into a hex constant
(avoids problems with leading zeroes)
8-bit constants max value 0x11111111, always fits in unsigned long
*/
#define _HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define _B8__(x)                                                               \
  \
((x & 0x0000000FLU) ? 1 : 0) \
+ ((x & 0x000000F0LU) ? 2 : 0) \
+ ((x & 0x00000F00LU) ? 4 : 0) \
+ ((x & 0x0000F000LU) ? 8 : 0) \
+ ((x & 0x000F0000LU) ? 16 : 0) \
+ ((x & 0x00F00000LU) ? 32 : 0) \
+ ((x & 0x0F000000LU) ? 64 : 0) \
+ ((x & 0xF0000000LU) ? 128 : 0)

/* *** user macros *** /

/* for upto 8-bit binary constants */
#define _B8(d) ((unsigned char)_B8__(_HEX__(d)))

/* for up to 16-bit binary constants, MSB first */
#define _B16(dmsb, dlsb)                                                       \
  \
(((unsigned short)_B8(dmsb) << 8) \
+ _B8(dlsb))

/* for up to 32-bit binary constants, MSB first */
#define _B32(dmsb, db2, db3, dlsb)                                             \
  \
(((unsigned long)_B8(dmsb) << 24) \
+ ((unsigned long)_B8(db2) << 16) \
+ ((unsigned long)_B8(db3) << 8) \
+ _B8(dlsb))