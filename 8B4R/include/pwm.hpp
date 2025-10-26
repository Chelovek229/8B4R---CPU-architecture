#pragma once
#include <cstdint>

class PWM {
public:
    PWM();

    void configure(uint16_t period, uint16_t duty);
    void tick();
    bool get_output() const;

private:
    uint16_t period;
    uint16_t duty;
    uint16_t counter;
    bool output;
};