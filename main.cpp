#include <iostream>
#include <set>
#include <bitset>
#include <array>
#include <algorithm>
#include <cmath>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

#include "led_line.pio.h"

#include "pioCommunicator.hpp"

#include "ledarray.hpp"

uint8_t value_for_row(const unsigned int iRow)
{
    return iRow / 2;
}

int main()
{
    stdio_init_all();
    std::cout << "LED Driver" << std::endl;

    LEDArray ledArr(pio0);

    // Set up an image
    std::array<uint8_t, 32 * 32> redSq, redTC;
    std::array<uint8_t, 32 * 32> greenSq, greenTC;
    std::array<uint8_t, 32 * 32> blueSq, blueTC;
    redSq.fill(0);
    greenSq.fill(0);
    blueSq.fill(0);
    redTC.fill(0);
    greenTC.fill(0);
    blueTC.fill(0);

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

                const unsigned int linearIdx = (ledArr.nCols * idxY) + idxX;
                redTC.at(linearIdx) = (iSquare & 1) ? value_for_row(idxY) : 0;
                greenTC.at(linearIdx) = (iSquare & 2) ? value_for_row(idxY) : 0;
                blueTC.at(linearIdx) = (iSquare & 4) ? value_for_row(idxY) : 0;
            }
        }
    }

    for (unsigned int iy = 0; iy < ledArr.nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < ledArr.nCols; ++ix)
        {
            redSq.at(ix + (ledArr.nCols * iy)) = ledArr.nFrames - value_for_row(iy);
            blueSq[ix + (ledArr.nCols * iy)] = ((ix) <= iy) * value_for_row(iy);
        }
    }

    std::cout << "Starting Main Loop" << std::endl;

    unsigned long itCount = 0;
    while (true)
    {
        // std::cout << "PIO running " << itCount << std::endl;
        ledArr.SendBuffer();
        itCount++;
        if ((itCount / 100) % 2)
        {
            // std::cout << "Sending Test Card " << itCount << std::endl;
            ledArr.UpdateBuffer(redTC, greenTC, blueTC);
        }
        else
        {

            // std::cout << "Sending Square " << itCount << std::endl;
            ledArr.UpdateBuffer(redSq, greenSq, blueSq);
        }
        // sleep_ms(1);
    }

    return 0;
}
