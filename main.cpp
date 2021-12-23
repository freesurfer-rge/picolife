#include <iostream>
#include <set>
#include <bitset>
#include <array>
#include <algorithm>
#include <cmath>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

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
    std::array<uint8_t, 32 * 32> red;
    std::array<uint8_t, 32 * 32> green;
    std::array<uint8_t, 32 * 32> blue;
    red.fill(0);
    green.fill(0);
    blue.fill(0);

    
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
                red.at(linearIdx) = (iSquare & 1) ? value_for_row(idxY) : 0;
                green.at(linearIdx) = (iSquare & 2) ? value_for_row(idxY) : 0;
                blue.at(linearIdx) = (iSquare & 4) ? value_for_row(idxY) : 0;
            }
        }
    }
    
    
    /*
    for (unsigned int iy = 0; iy < ledArr.nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < ledArr.nCols; ++ix)
        {
            red.at(ix + (ledArr.nCols * iy)) = iy;
            blue[ix + (ledArr.nCols * iy)] = ((ix) <= iy) * iy;
        }
    }
    */

   ledArr.UpdateBuffer(red, green, blue);

    std::cout << "Starting PIO" << std::endl;

    unsigned long itCount = 0;
    while (true)
    {
        std::cout << "PIO running " << itCount << std::endl;
        sleep_ms(1000);
        ++itCount;
    }

    return 0;
}
