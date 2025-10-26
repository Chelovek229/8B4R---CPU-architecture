// Простой ассемблер для 8B4R
// Simple assembler for 8B4R architecture
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

std::map<std::string, uint8_t> opcodes = {
    {"NOP", 0x00}, {"MOV", 0x01}, {"ADD", 0x02}, {"INC", 0x03}, {"HLT", 0xFF},
    {"SUB", 0x04}, {"AND", 0x05}, {"OR", 0x06}, {"XOR", 0x07}, {"CMP", 0x08},
    {"JMP", 0x10}, {"JZ", 0x11}, {"JC", 0x12}, {"PUSH", 0x20}, {"POP", 0x21}
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: assembler <input.asm> <output.bin>\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2], std::ios::binary);
    std::string line;
    while (std::getline(fin, line)) {
        std::istringstream iss(line);
        std::string op;
        iss >> op;
        if (op.empty() || op[0] == ';') continue;
        if (opcodes.count(op)) {
            fout.put(opcodes[op]);
        }
    }
    std::cout << "Assembled " << argv[1] << " to " << argv[2] << "\n";
    return 0;
}