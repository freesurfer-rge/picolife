#include <iostream>
#include <sstream>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "cells.hpp"
#include "sparselife/cellpattern.hpp"
#include "sparselife/sparselife.hpp"

#include "colourvector.hpp"

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

void SetInitialState(SparseLife::SparseLife &initialGrid)
{
#if 0
    {
        auto cellStream = std::stringstream(coeShipCells);
        SparseLife::CellPattern cp;
        cp.LoadFromStream(cellStream);
        cp.Translate(2, 4);
        cp.ExchangeXY();
        std::cout << "Adding cell count " << cp.GetCells().size() << std::endl;
        initialGrid.AddCells(cp.GetCells());
    }

    {
        auto cellStream = std::stringstream(fireShipCells);
        SparseLife::CellPattern cp;
        cp.LoadFromStream(cellStream);
        cp.Translate(16, 8);
        cp.FlipY();
        std::cout << "Adding cell count " << cp.GetCells().size() << std::endl;
        initialGrid.AddCells(cp.GetCells());
    }
#endif

#if 0
    {
        auto cellStream = std::stringstream(achimsp144Cells);
        SparseLife::CellPattern cp;
        cp.LoadFromStream(cellStream);
        cp.Translate(2, 1);
        std::cout << "Adding cell count " << cp.GetCells().size() << std::endl;
        initialGrid.AddCells(cp.GetCells());
    }
    {
        auto cellStream = std::stringstream(queenBeeShuttleCells);
        SparseLife::CellPattern cp;
        cp.LoadFromStream(cellStream);
        cp.Translate(4, 23);
        std::cout << "Adding cell count " << cp.GetCells().size() << std::endl;
        initialGrid.AddCells(cp.GetCells());
    }
#endif
    {
        auto cellStream = std::stringstream(lwssSynthRLE);
        SparseLife::CellPattern cp;
        cp.LoadRLEFromStream(cellStream);
        cp.Translate(14, 14);
        std::cout << "Adding cell count " << cp.GetCells().size() << std::endl;
        initialGrid.AddCells(cp.GetCells());
    }
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
    SetInitialState(grid);

    std::cout << "Starting core1" << std::endl;
    LEDDriver::LaunchOnCore1(&ledArr, 5);

    std::cout << "Starting Main Loop" << std::endl;

    unsigned long itCount = 0;
    auto img = ImageFromSparseLife(grid, itCount);
    img.SendToLEDArray(ledArr);
    sleep_ms(1000);
    while (true)
    {
        ++itCount;
        auto targetTime = make_timeout_time_ms(100);
        grid.Update();
        auto nxtImage = ImageFromSparseLife(grid, itCount);
        nxtImage.SendToLEDArray(ledArr);
        sleep_until(targetTime);
    }

    return 0;
}
