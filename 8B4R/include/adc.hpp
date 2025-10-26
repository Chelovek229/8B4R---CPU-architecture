#pragma once
#include <cstdint>
#include "interrupt_controller.hpp"

class ADC {
public:
    ADC();

    void start_conversion(uint8_t channel);
    bool conversion_complete() const;
    uint16_t get_result() const;

    void set_input(uint8_t channel, uint16_t value);
    void set_irq_vector(uint8_t vector);
    void poll(InterruptController& ic);

private:
    uint16_t inputs[4];
    uint16_t result;
    bool complete;
    uint8_t irq_vector;
};