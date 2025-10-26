#include "cpu.hpp"
#include "control_unit.hpp"

CPU::CPU(size_t programSize, size_t dataSize)
    : pc(0), sp(0), flags(0), halted(false), bus(programSize, dataSize) {}

void CPU::step() {
    if (halted) return;

    timer.tick();
    if (timer.interrupt_flag) {
        intc.request(0x10); // примерный вектор таймера
        timer.interrupt_flag = false;
    }

    if (intc.is_pending()) {
        // Сохраняем PC в стек, переходим по вектору
        stack.push(pc);
        pc = intc.irq_vector;
        intc.clear();
        return;
    }

    uint8_t opcode = bus.programMemory.read(pc++);
    ControlUnit::execute(*this, static_cast<Instruction>(opcode));
}

void CPU::load_program(const uint8_t* program, size_t size) {
    for (size_t i = 0; i < size && i < 256; ++i)
        RAM[i] = program[i];
}

void CPU::execute(bool verbose) {
    while (!halted) {
        step();
        if (verbose) {
            // Можно добавить вывод состояния регистров, памяти, периферии      
        }
    }
}
