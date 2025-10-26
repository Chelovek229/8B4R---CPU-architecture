// Генератор HEX-дампа
// HEX dump generator
#include <iostream>
#include <fstream>
#include <iomanip>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: hex_dump <file.bin>\n";
        return 1;
    }
    std::ifstream fin(argv[1], std::ios::binary);
    int addr = 0;
    uint8_t byte;
    while (fin.read(reinterpret_cast<char*>(&byte), 1)) {
        if (addr % 16 == 0) std::cout << std::setw(4) << std::setfill('0') << std::hex << addr << ": ";
        std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)byte << " ";
        if (++addr % 16 == 0) std::cout << "\n";
    }
    if (addr % 16 != 0) std::cout << "\n";
    return 0;
}