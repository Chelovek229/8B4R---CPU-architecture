#pragma once
#include <cstdint>
#include "interrupt_controller.hpp"

class GPIO {
public:
    static constexpr size_t PINS = 8;

    GPIO();

    void set_direction(uint8_t dir_mask);      // 1=output, 0=input
    void write_port(uint8_t value);
    uint8_t read_port() const;

    void write_pin(size_t idx, bool value);
    bool read_pin(size_t idx) const;

    void set_inputs(uint8_t input_mask);       // эмул€ци€ внешних сигналов

    void enable_irq(uint8_t irq_mask, uint8_t irq_vector);
    void poll(InterruptController& ic);

private:
    uint8_t dir;        // направление (1=out)
    uint8_t out_reg;    // выходы
    uint8_t in_reg;     // входы (симулируемые)
    uint8_t irq_mask;
    uint8_t irq_vector;
    bool irq_armed;
};