#include "disassembler.hpp"
#include <fstream>
#include <unordered_map>

static const std::unordered_map<uint8_t, std::string> opcode_map = {
    {0x00, "NOP"},
    {0x01, "MOV A, B"},
    {0x02, "ADD A, B"},
    {0x03, "INC A"},
    {0xFF, "HLT"}
};

bool disassemble(const std::string& binary_file, const std::string& output_file) {
    std::ifstream in(binary_file, std::ios::binary);
    if (!in) return false;

    std::ofstream out(output_file);
    if (!out) return false;

    uint8_t byte;
    size_t addr = 0;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) {
        auto it = opcode_map.find(byte);
        std::string name = it != opcode_map.end() ? it->second : "UNKNOWN";
        out << std::hex << addr << ": " << name << "\n";
        ++addr;
    }

    return true;
}
