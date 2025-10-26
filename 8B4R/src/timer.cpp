#include "timer.hpp"

Timer::Timer() : counter(0), reload(0), enabled(false), interrupt_flag(false) {}

void Timer::tick() {
    if (!enabled) return;
    if (counter == 0) {
        counter = reload;
        interrupt_flag = true;
    } else {
        --counter;
    }
}

void Timer::reset() {
    counter = reload;
    interrupt_flag = false;
}

void Timer::enable(bool en) {
    enabled = en;
}