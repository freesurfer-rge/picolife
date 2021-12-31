#include <iostream>

#include "cells.hpp"

#include "gridSetup.hpp"

void SetInitialState(SparseLife::SparseLife &targetGrid, const int patternSelect)
{
    switch (patternSelect)
    {
        case 0:
            

        default:
            std::cerr << "Pattern selector " << patternSelect << " unrecognised" << std::endl;
    }
}