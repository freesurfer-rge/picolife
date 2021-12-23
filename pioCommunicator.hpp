#pragma once

#include "hardware/pio.h"

class PIOCommunicator {
public:

    PIO pio;
    uint sm;
    uint pin_clk;
    uint pin_data;

    void write32blocking(const uint32_t* src, const size_t n);

    void waitTXdrain();
};