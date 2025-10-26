#include "stack.hpp"

Stack::Stack(size_t size) : data(size, 0), sp(0) {}

void Stack::push(uint8_t value) {
    if (sp < data.size())
        data[sp++] = value;
}

uint8_t Stack::pop() {
    if (sp > 0)
        return data[--sp];
    return 0;
}

uint8_t Stack::get_sp() const {
    return sp;
}