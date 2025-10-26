#pragma once
#include <cstdint>
#include "interrupt_controller.hpp"

class SPI {
public:
    SPI();

    void configure(uint32_t clk_div, uint8_t mode);
    uint8_t transfer(uint8_t byte);

    void receive_byte(uint8_t b); // эмуляция ответа ведомого
    bool rx_ready() const;
    uint8_t rx_read();

    void set_irq_vector(uint8_t vector);
    void poll(InterruptController& ic);

private:
    uint32_t clk_div;
    uint8_t mode;
    uint8_t rx;
    bool has_rx;
    uint8_t irq_vector;
};