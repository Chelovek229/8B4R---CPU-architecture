#pragma once
#include <cstdint>
#include "interrupt_controller.hpp"

class I2C {
public:
    I2C();

    bool start(uint8_t addr_rw);
    void stop();

    bool write_byte(uint8_t b);
    uint8_t read_byte(bool ack);

    void set_irq_vector(uint8_t vector);
    void poll(InterruptController& ic);

    void slave_provide_byte(uint8_t b);

private:
    bool bus_busy;
    uint8_t slave_resp;
    bool has_slave_resp;
    uint8_t irq_vector;
};