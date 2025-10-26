
    // Базовые блоки
    Bus bus;
    Timer timer;
    InterruptController intc;
    Stack stack;
#pragma once
#include <cstdint>
#include "bus.hpp"
#include "timer.hpp"
#include "interrupt_controller.hpp"
#include "stack.hpp"
#include "gpio.hpp"
#include "spi.hpp"
#include "uart.hpp"
#include "i2c.hpp"
#include "flash.hpp"
#include "clock_system.hpp"
#include "adc.hpp"
#include "dac.hpp"
#include "pwm.hpp"
#include "dma.hpp"

class CPU {
public:
    // Основные регистры
    uint8_t registers[4];
    uint8_t pc;
    uint8_t sp;
    uint8_t flags;
    bool halted;

    // Периферия
    GPIO gpio;
    SPI spi;
    UART uart;
    I2C i2c;

    // Новые модули
    Flash flash;
    ClockSystem clock;
    ADC adc;
    DAC dac;
    PWM pwm;
    DMA dma;

    CPU(size_t programSize, size_t dataSize, size_t flashSize = 1024);

    void step();
    void load_program(const uint8_t* program, size_t size);
    void execute(bool verbose = false);
};
