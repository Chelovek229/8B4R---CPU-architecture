#pragma once
#include "cpu.hpp"

class ControlUnit {
public:
    static void execute(CPU& cpu, Instruction inst);
};