#include "pwm.hpp"

PWM::PWM() : period(100), duty(50), counter(0), output(false) {}

void PWM::configure(uint16_t p, uint16_t d) {
    period = p;
    duty = d;
}

void PWM::tick() {
    if (++counter >= period) counter = 0;
    output = (counter < duty);
}

bool PWM::get_output() const { return output; }