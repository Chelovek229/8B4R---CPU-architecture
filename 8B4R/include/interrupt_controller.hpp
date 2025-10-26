#pragma once
#include <cstdint>

class InterruptController {
public:
    bool irq_pending;
    uint8_t irq_vector;

    InterruptController();

    void request(uint8_t vector);
    void clear();
    bool is_pending() const;
};