#pragma once

#include "hardware/pio.h"

namespace LEDDriver
{
    class PIOCommunicator
    {
    public:
        PIO pio;
        uint sm;

        void write32blocking(const uint32_t *src, const size_t n);

        void waitTXdrain();
    };
}