#pragma once
#include "memory.hpp"

class Bus {
public:
    Memory programMemory;
    Memory dataMemory;

    Bus(size_t programSize, size_t dataSize);
};