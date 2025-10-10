#pragma once
#include <cstdint>
#include "instructions.hpp"

struct CPU {
    uint8_t A = 0, B = 0, C = 0, D = 0;
    uint8_t PC = 0;
    uint8_t RAM[256] = {};
    bool halted = false;

    void load_program(const uint8_t* program, size_t size);
    void execute(bool verbose = false);
};
