#include "clock_system.hpp"

ClockSystem::ClockSystem(uint32_t freq_hz) : frequency(freq_hz), ticks(0) {}

void ClockSystem::set_frequency(uint32_t freq_hz) { frequency = freq_hz; }
uint32_t ClockSystem::get_frequency() const { return frequency; }

void ClockSystem::tick() { ++ticks; }