#pragma once
#include <cstdint>
#include <string>

enum class Instruction : uint8_t {
    NOP = 0x00,
    MOV_A_B = 0x01,
    ADD_A_B = 0x02,
    INC_A = 0x03,
    HLT = 0xFF
};

std::string instruction_to_string(Instruction inst);
