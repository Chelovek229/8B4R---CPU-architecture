#include "gpio.hpp"

GPIO::GPIO()
    : dir(0), out_reg(0), in_reg(0), irq_mask(0), irq_vector(0), irq_armed(true) {}

void GPIO::set_direction(uint8_t dir_mask) {
    dir = dir_mask;
}

void GPIO::write_port(uint8_t value) {
    out_reg = (out_reg & ~dir) | (value & dir);
}

uint8_t GPIO::read_port() const {
    return (out_reg & dir) | (in_reg & ~dir);
}

void GPIO::write_pin(size_t idx, bool value) {
    if (idx >= PINS) return;
    uint8_t mask = (1u << idx);
    if (dir & mask) {
        if (value) out_reg |= mask; else out_reg &= ~mask;
    }
}

bool GPIO::read_pin(size_t idx) const {
    if (idx >= PINS) return false;
    uint8_t mask = (1u << idx);
    return (read_port() & mask) != 0;
}

void GPIO::set_inputs(uint8_t input_mask) {
    in_reg = (in_reg & dir) | (input_mask & ~dir);
}

void GPIO::enable_irq(uint8_t mask, uint8_t vector) {
    irq_mask = mask;
    irq_vector = vector;
    irq_armed = true;
}

void GPIO::poll(InterruptController& ic) {
    if (!irq_armed || irq_mask == 0) return;
    uint8_t active = in_reg & irq_mask;
    if (active) {
        ic.request(irq_vector);
        irq_armed = false;
    }
}