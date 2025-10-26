#pragma once
#include <cstdint>

class Timer {
public:
    uint8_t counter;
    uint8_t reload;
    bool enabled;
    bool interrupt_flag;

    Timer();

    void tick();
    void reset();
    void enable(bool en);
};