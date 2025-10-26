#pragma once
#include <cstdint>
#include <vector>

class Stack {
    std::vector<uint8_t> data;
    uint8_t sp;
public:
    Stack(size_t size = 16);

    void push(uint8_t value);
    uint8_t pop();
    uint8_t get_sp() const;
};