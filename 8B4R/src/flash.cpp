#include "flash.hpp"

Flash::Flash(size_t size) : data(size, 0xFF), programmed(false) {}

uint8_t Flash::read(uint16_t address) const {
    return data[address];
}

void Flash::program(const std::vector<uint8_t>& d) {
    if (!programmed && d.size() <= data.size()) {
        std::copy(d.begin(), d.end(), data.begin());
        programmed = true;
    }
}

void Flash::erase() {
    std::fill(data.begin(), data.end(), 0xFF);
    programmed = false;
}