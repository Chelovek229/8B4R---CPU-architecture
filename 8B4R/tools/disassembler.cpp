// Дизассемблер для 8B4R
// Disassembler for 8B4R architecture
#include <iostream>
#include <fstream>
#include <map>

std::map<uint8_t, std::string> mnemonics = {
    {0x00, "NOP"}, {0x01, "MOV A, B"}, {0x02, "ADD A, B"}, {0x03, "INC A"}, {0xFF, "HLT"},
    {0x04, "SUB A, B"}, {0x05, "AND A, B"}, {0x06, "OR A, B"}, {0x07, "XOR A, B"}, {0x08, "CMP A, B"},
    {0x10, "JMP"}, {0x11, "JZ"}, {0x12, "JC"}, {0x20, "PUSH A"}, {0x21, "POP A"}
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: disassembler <input.bin> <output.asm>\n";
        return 1;
    }
    std::ifstream fin(argv[1], std::ios::binary);
    std::ofstream fout(argv[2]);
    uint8_t byte;
    while (fin.read(reinterpret_cast<char*>(&byte), 1)) {
        if (mnemonics.count(byte))
            fout << mnemonics[byte] << "\n";
        else
            fout << "DB 0x" << std::hex << (int)byte << "\n";
    }
    std::cout << "Disassembled " << argv[1] << " to " << argv[2] << "\n";
    return 0;
}