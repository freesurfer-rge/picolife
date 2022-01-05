#include <iostream>
#include <sstream>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"

#include "sparselife/sparselife.hpp"

#include "colourvector.hpp"
#include "gridSetup.hpp"

#include "leddriver/ledarray.hpp"
#include "leddriver/ledimage.hpp"
#include "leddriver/utility.hpp"

ColourVector rV(0.2f, 0.3f, 0.1f, 4, 6);
ColourVector gV(0.1f, -0.3f, 0.3f, 4, 8);
ColourVector bV(-0.2f, 0.2f, 0.5f, 6, 7);

LEDDriver::LEDImage ImageFromSparseLife(const SparseLife::SparseLife &grid, const unsigned long itCount)
{
    LEDDriver::LEDImage result;

    for (auto c : grid.GetCells())
    {
        const int16_t ix = c.first;
        const int16_t iy = c.second;
        auto r = rV.GetColour(ix, iy, itCount);
        auto g = gV.GetColour(ix, iy, itCount);
        auto b = bV.GetColour(ix, iy, itCount);
        result.SetPixel(ix, iy, r, g, b);
    }

    return result;
}

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    std::cout << "LED Driver" << std::endl;

    LEDDriver::LEDArray ledArr(pio0);

    // Set up Life Board
    std::cout << "Creating the grid" << std::endl;
    SparseLife::SparseLife grid(LEDDriver::LEDArray::nCols, LEDDriver::LEDArray::nRows, true, true);

    // Populate the Life board
    std::cout << "Adding initial cells to grid" << std::endl;
    SetInitialState(grid, 0);

    std::cout << "Starting core1" << std::endl;
    LEDDriver::LaunchOnCore1(&ledArr, 5);

    std::cout << "Starting Main Loop" << std::endl;

    unsigned long itCount = 0;
    const unsigned long maxPatternIts = 1000;
    unsigned char nextPattern = 0;
    const unsigned char maxPattern = 5;
    auto img = ImageFromSparseLife(grid, itCount);
    img.SendToLEDArray(ledArr);
    sleep_ms(1000);
    while (true)
    {
        ++itCount;
        auto ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT || itCount == maxPatternIts)
        {
            nextPattern = (nextPattern + 1) % maxPattern;
            if (ch != PICO_ERROR_TIMEOUT)
            {
                nextPattern = ch - '0';
            }
            SetInitialState(grid, nextPattern);
            itCount = 0;
            sleep_ms(1000);
        }
        auto targetTime = make_timeout_time_ms(50);
        grid.Update();
        auto nxtImage = ImageFromSparseLife(grid, itCount);
        nxtImage.SendToLEDArray(ledArr);
        sleep_until(targetTime);
    }

    return 0;
}
