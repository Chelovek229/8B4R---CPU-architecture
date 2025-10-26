#include "control_unit.hpp"
#include "alu.hpp"

void ControlUnit::execute(CPU& cpu, Instruction inst) {
    switch (inst) {
    case Instruction::NOP:
        break;
    case Instruction::MOV_A_B:
        cpu.registers[0] = cpu.registers[1];
        break;
    case Instruction::ADD_A_B:
        cpu.registers[0] = ALU::add(cpu.registers[0], cpu.registers[1], cpu.flags);
        break;
    case Instruction::INC_A:
        cpu.registers[0] = ALU::add(cpu.registers[0], 1, cpu.flags);
        break;
    case Instruction::SUB_A_B:
        cpu.registers[0] = ALU::sub(cpu.registers[0], cpu.registers[1], cpu.flags);
        break;
    case Instruction::AND_A_B:
        cpu.registers[0] = ALU::and_op(cpu.registers[0], cpu.registers[1]);
        break;
    case Instruction::OR_A_B:
        cpu.registers[0] = ALU::or_op(cpu.registers[0], cpu.registers[1]);
        break;
    case Instruction::XOR_A_B:
        cpu.registers[0] = ALU::xor_op(cpu.registers[0], cpu.registers[1]);
        break;
    case Instruction::CMP_A_B:
        ALU::compare(cpu.registers[0], cpu.registers[1], cpu.flags);
        break;
    case Instruction::JMP:
        cpu.pc = cpu.bus.programMemory.read(cpu.pc); // Пример: следующий байт — адрес
        break;
    case Instruction::JZ:
        if (cpu.flags & FLAG_Z)
            cpu.pc = cpu.bus.programMemory.read(cpu.pc);
        else
            cpu.pc++;
        break;
    case Instruction::JC:
        if (cpu.flags & FLAG_C)
            cpu.pc = cpu.bus.programMemory.read(cpu.pc);
        else
            cpu.pc++;
        break;
    case Instruction::PUSH_A:
        cpu.stack.push(cpu.registers[0]);
        break;
    case Instruction::POP_A:
        cpu.registers[0] = cpu.stack.pop();
        break;
    case Instruction::HLT:
        cpu.halted = true;
        break;
    default:
        cpu.halted = true;
        break;
    }
}