#include <iostream>
#include <sstream>

#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "pioCommunicator.hpp"

#include "cells.hpp"
#include "cellpattern.hpp"
#include "sparselife.hpp"

#include "colourvector.hpp"

#include "ledarray.hpp"
#include "ledimage.hpp"

void core1Entry()
{
    uint32_t fifo_value = multicore_fifo_pop_blocking();
    LEDArray *ledArr = reinterpret_cast<LEDArray *>(fifo_value);

    while (true)
    {
        auto targetTime = make_timeout_time_ms(5);
        ledArr->SendBuffer();
        sleep_until(targetTime);
    }
}

uint8_t value_for_row(const unsigned int iRow)
{
    return iRow / 2;
}

LEDImage CreateTestCard()
{
    LEDImage result;

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

                uint8_t r = (iSquare & 1) ? value_for_row(idxY) : 0;
                uint8_t g = (iSquare & 2) ? value_for_row(idxY) : 0;
                uint8_t b = (iSquare & 4) ? value_for_row(idxY) : 0;
                result.SetPixel(idxX, idxY, r, g, b);
            }
        }
    }

    return result;
}

LEDImage CreateSquareDiagonal()
{
    LEDImage result;

    for (unsigned int iy = 0; iy < LEDArray::nRows; ++iy)
    {
        for (unsigned int ix = 0; ix < LEDArray::nCols; ++ix)
        {
            uint8_t r = LEDArray::nFrames - value_for_row(iy);
            uint8_t g = ((LEDArray::nCols - 1) - ix == iy) ? 255 : 0;
            uint8_t b = ((ix) <= iy) * value_for_row(iy);

            result.SetPixel(ix, iy, r, g, b);
        }
    }

    return result;
}

LEDImage ImageFromSparseLife(const SparseLife &grid, const unsigned long itCount)
{
    LEDImage result;

    for (auto c : grid.GetCells())
    {
        result.SetPixel(c.first, c.second, 8, 0, 8);
    }

    return result;
}

int main()
{
    stdio_init_all();
    std::cout << "LED Driver" << std::endl;

    LEDArray *ledArr = new LEDArray(pio0);

    // Set up Life Board
    SparseLife grid(LEDArray::nCols, LEDArray::nRows, true, true);

    // Setup a glider
    auto cellStream = std::stringstream(coeShipCells);
    CellPattern cp;
    cp.LoadFromStream(cellStream);
    cp.Translate(4, 4);
    cp.ExchangeXY();

    grid.AddCells(cp.GetCells());

    std::cout << "Starting core1" << std::endl;
    multicore_launch_core1(core1Entry);
    std::cout << "Sending address of array object" << std::endl;
    multicore_fifo_push_blocking(reinterpret_cast<uint32_t>(ledArr));

    std::cout << "Starting Main Loop" << std::endl;

    unsigned long itCount = 0;
    auto img = ImageFromSparseLife(grid, itCount);
    img.SendToLEDArray(*ledArr);
    sleep_ms(1000);
    while (true)
    {
        ++itCount;
        auto targetTime = make_timeout_time_ms(100);
        grid.Update();
        auto nxtImage = ImageFromSparseLife(grid, itCount);
        nxtImage.SendToLEDArray(*ledArr);
        sleep_until(targetTime);
    }

    return 0;
}
