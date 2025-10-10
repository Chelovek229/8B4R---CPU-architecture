#include <iostream>
#include <fstream>
#include "cpu.hpp"

int main() {
    CPU cpu;

    std::ifstream file("tools/test_program.bin", std::ios::binary);
    if (!file) {
        std::cerr << "Не удалось открыть бинарную программу.\n";
        return 1;
    }

    uint8_t buffer[256] = {};
    file.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
    size_t size = file.gcount();

    cpu.B = 5;
    cpu.load_program(buffer, size);
    cpu.execute(true); // true — логировать инструкции

    std::cout << "Регистр A: " << (int)cpu.A << std::endl;
    return 0;
}
