#pragma once
#include <cstdint>

class ClockSystem {
public:
    ClockSystem(uint32_t freq_hz = 1000000);

    void set_frequency(uint32_t freq_hz);
    uint32_t get_frequency() const;

    void tick(); // эмуляция такта

private:
    uint32_t frequency;
    uint64_t ticks;
};