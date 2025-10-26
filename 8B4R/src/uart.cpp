#include "uart.hpp"

UART::UART() : baud(9600), irq_vector(0) {}

void UART::configure(uint32_t baudrate) {
    baud = baudrate;
}

void UART::send(uint8_t b) {
    tx_buf.push_back(b);
}

void UART::receive_byte(uint8_t b) {
    rx_buf.push_back(b);
}

bool UART::rx_available() const {
    return !rx_buf.empty();
}

uint8_t UART::rx_read() {
    if (rx_buf.empty()) return 0;
    uint8_t v = rx_buf.front();
    rx_buf.pop_front();
    return v;
}

void UART::set_irq_vector(uint8_t vector) {
    irq_vector = vector;
}

void UART::poll(InterruptController& ic) {
    if (!rx_buf.empty() && irq_vector != 0) {
        ic.request(irq_vector);
    }
}