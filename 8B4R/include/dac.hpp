#pragma once
#include <cstdint>

class DAC {
public:
    DAC();

    void write(uint16_t value);
    uint16_t get_output() const;

private:
    uint16_t output;
};