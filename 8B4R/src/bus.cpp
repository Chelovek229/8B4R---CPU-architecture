#include "bus.hpp"

Bus::Bus(size_t programSize, size_t dataSize)
    : programMemory(programSize), dataMemory(dataSize) {}