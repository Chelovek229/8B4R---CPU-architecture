#include "dac.hpp"

DAC::DAC() : output(0) {}
void DAC::write(uint16_t value) { output = value; }
uint16_t DAC::get_output() const { return output; }