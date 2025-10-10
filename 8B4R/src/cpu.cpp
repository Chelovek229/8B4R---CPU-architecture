#include "cpu.hpp"
#include <iostream>
#include "instructions.hpp"

void CPU::load_program(const uint8_t* program, size_t size) {
    for (size_t i = 0; i < size && i < 256; ++i)
        RAM[i] = program[i];
}

void CPU::execute(bool verbose) {
    while (!halted) {
        Instruction inst = static_cast<Instruction>(RAM[PC++]);

        if (verbose)
            std::cout << "[" << (int)(PC - 1) << "] "
            << instruction_to_string(inst) << "\n";

        switch (inst) {
        case Instruction::NOP: break;
        case Instruction::MOV_A_B: A = B; break;
        case Instruction::ADD_A_B: A += B; break;
        case Instruction::INC_A: ++A; break;
        case Instruction::HLT: halted = true; break;
        default: halted = true; break;
        }
    }
}
