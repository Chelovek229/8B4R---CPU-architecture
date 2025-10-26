#include "adc.hpp"

ADC::ADC() : result(0), complete(false), irq_vector(0) {
    for (auto& v : inputs) v = 0;
}

void ADC::start_conversion(uint8_t channel) {
    if (channel < 4) {
        result = inputs[channel];
        complete = true;
    }
}

bool ADC::conversion_complete() const { return complete; }
uint16_t ADC::get_result() const { return result; }

void ADC::set_input(uint8_t channel, uint16_t value) {
    if (channel < 4) inputs[channel] = value;
}

void ADC::set_irq_vector(uint8_t vector) { irq_vector = vector; }

void ADC::poll(InterruptController& ic) {
    if (complete && irq_vector) {
        ic.request(irq_vector);
        complete = false;
    }
}