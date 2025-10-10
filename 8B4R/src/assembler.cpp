#include "assembler.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

static const std::unordered_map<std::string, uint8_t> instruction_map = {
    {"NOP", 0x00},
    {"MOV A, B", 0x01},
    {"ADD A, B", 0x02},
    {"INC A", 0x03},
    {"HLT", 0xFF}
};

bool assemble(const std::string& input_file, const std::string& output_file) {
    std::ifstream in(input_file);
    if (!in) return false;

    std::ofstream out(output_file, std::ios::binary);
    if (!out) return false;

    std::string line;
    while (std::getline(in, line)) {
        // Очистка строки от пробелов и комментариев
        size_t comment = line.find(';');
        if (comment != std::string::npos) line = line.substr(0, comment);

        while (!line.empty() && isspace(line.back())) line.pop_back();
        while (!line.empty() && isspace(line.front())) line.erase(line.begin());

        if (line.empty()) continue;

        if (instruction_map.contains(line)) {
            uint8_t opcode = instruction_map.at(line);
            out.write(reinterpret_cast<char*>(&opcode), 1);
        }
        else {
            std::cerr << "Неизвестная команда: " << line << std::endl;
            return false;
        }
    }

    return true;
}
