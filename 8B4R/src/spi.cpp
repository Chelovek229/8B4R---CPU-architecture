#include "spi.hpp"

SPI::SPI() : clk_div(0), mode(0), rx(0), has_rx(false), irq_vector(0) {}

void SPI::configure(uint32_t c, uint8_t m) {
    clk_div = c;
    mode = m & 3;
}

uint8_t SPI::transfer(uint8_t byte) {
    uint8_t response = has_rx ? rx : 0xFF;
    has_rx = false;
    return response;
}

void SPI::receive_byte(uint8_t b) {
    rx = b;
    has_rx = true;
}

bool SPI::rx_ready() const { return has_rx; }

uint8_t SPI::rx_read() {
    has_rx = false;
    return rx;
}

void SPI::set_irq_vector(uint8_t vector) {
    irq_vector = vector;
}

void SPI::poll(InterruptController& ic) {
    if (has_rx && irq_vector != 0) {
        ic.request(irq_vector);
    }
}