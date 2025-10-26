#pragma once
#include <vector>
#include <cstdint>

class Memory {
private:
    std::vector<uint8_t> data;
public:
    Memory(size_t size);
    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
};