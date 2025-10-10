#include "instructions.hpp"

std::string instruction_to_string(Instruction inst) {
    switch (inst) {
    case Instruction::NOP: return "NOP";
    case Instruction::MOV_A_B: return "MOV A, B";
    case Instruction::ADD_A_B: return "ADD A, B";
    case Instruction::INC_A: return "INC A";
    case Instruction::HLT: return "HLT";
    default: return "UNKNOWN";
    }
}
