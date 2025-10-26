#pragma once
#include <cstdint>

class ALU {
public:
    static uint8_t add(uint8_t a, uint8_t b, uint8_t& flags);
    static uint8_t sub(uint8_t a, uint8_t b, uint8_t& flags);
    static uint8_t and_op(uint8_t a, uint8_t b);
    static uint8_t or_op(uint8_t a, uint8_t b);
    static uint8_t xor_op(uint8_t a, uint8_t b);
    static bool compare(uint8_t a, uint8_t b, uint8_t& flags);
};