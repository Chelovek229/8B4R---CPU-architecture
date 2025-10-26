#pragma once
#include <cstdint>

class DMA {
public:
    DMA();

    void start(uint8_t* src, uint8_t* dst, size_t size);
    bool is_active() const;
    void tick();

private:
    uint8_t* src_ptr;
    uint8_t* dst_ptr;
    size_t remaining;
    bool active;
};