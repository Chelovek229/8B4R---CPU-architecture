#include "dma.hpp"

DMA::DMA() : src_ptr(nullptr), dst_ptr(nullptr), remaining(0), active(false) {}

void DMA::start(uint8_t* src, uint8_t* dst, size_t size) {
    src_ptr = src;
    dst_ptr = dst;
    remaining = size;
    active = true;
}

bool DMA::is_active() const { return active; }

void DMA::tick() {
    if (active && remaining) {
        *dst_ptr++ = *src_ptr++;
        --remaining;
        if (remaining == 0) active = false;
    }
}#include "flash.hpp"
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
};#include "flash.hpp"
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
};