#pragma once
#include <vector>
#include <cstdint>

class Flash {
public:
    Flash(size_t size);

    uint8_t read(uint16_t address) const;
    void program(const std::vector<uint8_t>& data); // ���������������� (������ ���� ���)
    void erase(); // ������� (�����������)

private:
    std::vector<uint8_t> data;
    bool programmed;
};