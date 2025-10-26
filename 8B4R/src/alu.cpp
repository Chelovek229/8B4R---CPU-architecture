#include "flash.hpp"
#include "clock_system.hpp"
#include "adc.hpp"
#include "dac.hpp"
#include "pwm.hpp"
#include "dma.hpp"
// ...
class CPU {
    // ...
    Flash flash;
    ClockSystem clock;
    ADC adc;
    DAC dac;
    PWM pwm;
    DMA dma;
    // ...
};#include "alu.hpp"
#include "cpu.hpp"

uint8_t ALU::add(uint8_t a, uint8_t b, uint8_t& flags) {
    uint16_t result = a + b;
    flags = 0;
    if ((result & 0xFF) == 0) flags |= FLAG_Z;
    if (result > 0xFF) flags |= FLAG_C;
    if (result & 0x80) flags |= FLAG_S;
    if (((a ^ ~b) & (a ^ result) & 0x80) != 0) flags |= FLAG_V;
    return static_cast<uint8_t>(result);
}

uint8_t ALU::sub(uint8_t a, uint8_t b, uint8_t& flags) {
    uint16_t result = a - b;
    flags = 0;
    if ((result & 0xFF) == 0) flags |= FLAG_Z;
    if (result > 0xFF) flags |= FLAG_C;
    if (result & 0x80) flags |= FLAG_S;
    if (((a ^ b) & (a ^ result) & 0x80) != 0) flags |= FLAG_V;
    return static_cast<uint8_t>(result);
}

uint8_t ALU::and_op(uint8_t a, uint8_t b) {
    return a & b;
}

uint8_t ALU::or_op(uint8_t a, uint8_t b) {
    return a | b;
}

uint8_t ALU::xor_op(uint8_t a, uint8_t b) {
    return a ^ b;
}

bool ALU::compare(uint8_t a, uint8_t b, uint8_t& flags) {
    uint8_t result = a - b;
    flags = 0;
    if (result == 0) flags |= FLAG_Z;
    if (result & 0x80) flags |= FLAG_S;
    return result == 0;
}