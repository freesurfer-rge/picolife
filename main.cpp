#include <array>
#include <iostream>
#include <memory>

#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "pioCommunicator.hpp"

#include "ledarray.hpp"

void core1Entry()
{
    uint32_t fifo_value = multicore_fifo_pop_blocking();
    LEDArray *ledArr = reinterpret_cast<LEDArray *>(fifo_value);

    while (true)
    {
        auto targetTime = make_timeout_time_ms(10);
        ledArr->SendBuffer();
        sleep_until(targetTime);
    }
}

uint8_t value_for_row(const unsigned int iRow)
{
    return iRow / 2;
}

typedef std::array<uint8_t, 32 * 32> Channel;

int main()
{
    stdio_init_all();
    std::cout << "LED Driver" << std::endl;

    LEDArray *ledArr = new LEDArray(pio0);

    // Set up an image
    auto redSq = std::make_unique<Channel>();
    auto redTC = std::make_unique<Channel>();
    auto greenSq = std::make_unique<Channel>();
    auto greenTC = std::make_unique<Channel>();
    auto blueSq = std::make_unique<Channel>();
    auto blueTC = std::make_unique<Channel>();

    redSq->fill(0);
    greenSq->fill(0);
    blueSq->fill(0);
    redTC->fill(0);
    greenTC->fill(0);
    blueTC->fill(0);

    for (unsigned int iSquare = 0; iSquare < 64; iSquare++)
    {
        unsigned int sx = iSquare % 8;
        unsigned int sy = iSquare / 8;

        for (unsigned int ix = 0; ix < 4; ++ix)
        {
            for (unsigned int iy = 0; iy < 4; ++iy)
            {
                unsigned int idxX = sx * 4 + ix;
                unsigned int idxY = sy * 4 + iy;

                const unsigned int linearIdx = (ledArr->nCols * idxY) + idxX;
                redTC->at(linearIdx) = (iSquare & 1) ? value_for_row(idxY) : 0;
                greenTC->at(linearIdx) = (iSquare & 2) ? value_for_row(idxY) : 0;
                blueTC->at(linearIdx) = (iSquare & 4) ? value_for_row(idxY) : 0;
            }
        }
    }

    for (unsigned int iy = 0; iy < ledArr->nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < ledArr->nCols; ++ix)
        {
            redSq->at(ix + (ledArr->nCols * iy)) = ledArr->nFrames - value_for_row(iy);
            blueSq->at(ix + (ledArr->nCols * iy)) = ((ix) <= iy) * value_for_row(iy);
        }
    }
    std::cout << "Starting core1" << std::endl;
    multicore_launch_core1(core1Entry);
    std::cout << "Sending address of array object" << std::endl;
    multicore_fifo_push_blocking(reinterpret_cast<uint32_t>(ledArr));

    std::cout << "Starting Main Loop" << std::endl;

    unsigned long itCount = 0;
    while (true)
    {
        if (itCount % 2)
        {
            ledArr->UpdateBuffer(*redTC, *greenTC, *blueTC);
        }
        else
        {
            ledArr->UpdateBuffer(*redSq, *greenSq, *blueSq);
        }
        itCount++;
        sleep_ms(1000);
    }

    return 0;
}
