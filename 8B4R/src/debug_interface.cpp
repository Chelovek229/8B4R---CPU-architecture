#include "debug_interface.hpp"

// Пример реализации для JTAG
void JTAG::halt(CPU& cpu) { cpu.halted = true; }
void JTAG::resume(CPU& cpu) { cpu.halted = false; }
uint8_t JTAG::read_register(CPU& cpu, int reg) {
    switch (reg) {
    case 0: return cpu.A;
    case 1: return cpu.B;
    case 2: return cpu.C;
    case 3: return cpu.D;
    default: return 0;
    }
}
void JTAG::write_register(CPU& cpu, int reg, uint8_t value) {
    switch (reg) {
    case 0: cpu.A = value; break;
    case 1: cpu.B = value; break;
    case 2: cpu.C = value; break;
    case 3: cpu.D = value; break;
    }
}
uint8_t JTAG::read_memory(CPU& cpu, uint16_t addr) { return cpu.RAM[addr]; }
void JTAG::write_memory(CPU& cpu, uint16_t addr, uint8_t value) { cpu.RAM[addr] = value; }
void JTAG::set_breakpoint(CPU& cpu, uint16_t addr) { /* Реализуйте список точек останова */ }

// SWD и ICE могут делегировать JTAG или иметь свои особенности
void SWD::halt(CPU& cpu) { JTAG::halt(cpu); }
void SWD::resume(CPU& cpu) { JTAG::resume(cpu); }
uint8_t SWD::read_register(CPU& cpu, int reg) { return JTAG::read_register(cpu, reg); }
void SWD::write_register(CPU& cpu, int reg, uint8_t value) { JTAG::write_register(cpu, reg, value); }
uint8_t SWD::read_memory(CPU& cpu, uint16_t addr) { return JTAG::read_memory(cpu, addr); }
void SWD::write_memory(CPU& cpu, uint16_t addr, uint8_t value) { JTAG::write_memory(cpu, addr, value); }
void SWD::set_breakpoint(CPU& cpu, uint16_t addr) { JTAG::set_breakpoint(cpu, addr); }

void ICE::halt(CPU& cpu) { JTAG::halt(cpu); }
void ICE::resume(CPU& cpu) { JTAG::resume(cpu); }
uint8_t ICE::read_register(CPU& cpu, int reg) { return JTAG::read_register(cpu, reg); }
void ICE::write_register(CPU& cpu, int reg, uint8_t value) { JTAG::write_register(cpu, reg, value); }
uint8_t ICE::read_memory(CPU& cpu, uint16_t addr) { return JTAG::read_memory(cpu, addr); }
void ICE::write_memory(CPU& cpu, uint16_t addr, uint8_t value) { JTAG::write_memory(cpu, addr, value); }
void ICE::set_breakpoint(CPU& cpu, uint16_t addr) { JTAG::set_breakpoint(cpu, addr); }