#pragma once
#include <cstdint>
#include <deque>
#include "interrupt_controller.hpp"

class UART {
public:
    UART();

    void configure(uint32_t baudrate);

    void send(uint8_t b);
    void receive_byte(uint8_t b);

    bool rx_available() const;
    uint8_t rx_read();

    void set_irq_vector(uint8_t vector);
    void poll(InterruptController& ic);

private:
    uint32_t baud;
    std::deque<uint8_t> rx_buf;
    std::deque<uint8_t> tx_buf;
    uint8_t irq_vector;
};