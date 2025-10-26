#include "i2c.hpp"

I2C::I2C() : bus_busy(false), slave_resp(0), has_slave_resp(false), irq_vector(0) {}

bool I2C::start(uint8_t addr_rw) {
    bus_busy = true;
    return true;
}

void I2C::stop() {
    bus_busy = false;
}

bool I2C::write_byte(uint8_t b) {
    return true;
}

uint8_t I2C::read_byte(bool ack) {
    if (has_slave_resp) {
        has_slave_resp = false;
        return slave_resp;
    }
    return 0xFF;
}

void I2C::set_irq_vector(uint8_t vector) {
    irq_vector = vector;
}

void I2C::poll(InterruptController& ic) {
    if (has_slave_resp && irq_vector != 0) {
        ic.request(irq_vector);
    }
}

void I2C::slave_provide_byte(uint8_t b) {
    slave_resp = b;
    has_slave_resp = true;
}