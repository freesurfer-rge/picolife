#include <iostream>
#include <random>
#include <sstream>

#include "pico/time.h"

#include "sparselife/cellpattern.hpp"

#include "cells.hpp"

#include "gridSetup.hpp"

void SetInitialState(SparseLife::SparseLife &targetGrid, const int patternSelect)
{
    switch (patternSelect)
    {
    case 0:
        std::cout << "Adding soup" << std::endl;
        targetGrid.ClearCells();
        {
            auto seed = to_ms_since_boot(get_absolute_time());
            std::default_random_engine generator(seed);
            std::uniform_int_distribution<int> distX(0, targetGrid.nx - 1);
            std::uniform_int_distribution<int> distY(0, targetGrid.ny - 1);
            for (auto i = 0; i < 500; ++i)
            {
                auto x = distX(generator);
                auto y = distY(generator);

                targetGrid.AddCell(SparseLife::Cell(x, y));
            }
        }
        break;

    case 1:
        std::cout << "Adding Coe and Fireships" << std::endl;
        targetGrid.ClearCells();
        {
            auto cellStream = std::stringstream(coeShipCells);
            SparseLife::CellPattern cp;
            cp.LoadFromStream(cellStream);
            cp.Translate(2, 4);
            cp.ExchangeXY();
            targetGrid.AddCells(cp.GetCells());
        }

        {
            auto cellStream = std::stringstream(fireShipCells);
            SparseLife::CellPattern cp;
            cp.LoadFromStream(cellStream);
            cp.Translate(16, 8);
            cp.FlipY();
            targetGrid.AddCells(cp.GetCells());
        }
        break;

    case 2:
        std::cout << "Adding oscillators" << std::endl;
        targetGrid.ClearCells();
        {
            auto cellStream = std::stringstream(achimsp144Cells);
            SparseLife::CellPattern cp;
            cp.LoadFromStream(cellStream);
            cp.Translate(2, 1);
            targetGrid.AddCells(cp.GetCells());
        }
        {
            auto cellStream = std::stringstream(queenBeeShuttleCells);
            SparseLife::CellPattern cp;
            cp.LoadFromStream(cellStream);
            cp.Translate(4, 23);
            targetGrid.AddCells(cp.GetCells());
        }
        break;

    case 3:
        std::cout << "Adding lobster" << std::endl;
        targetGrid.ClearCells();
        {
            auto cellStream = std::stringstream(lobsterRLE);
            SparseLife::CellPattern cp;
            cp.LoadRLEFromStream(cellStream);
            cp.Translate(0, 0);
            targetGrid.AddCells(cp.GetCells());
        }
        break;

    default:
        std::cout << "Pattern selector " << patternSelect << " unrecognised" << std::endl;
    }
}