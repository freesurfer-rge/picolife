#include "cellpattern.hpp"

CellPattern::CellPattern() : activeCells(std::make_unique<std::set<CellPattern::Cell>>())
{
    // Nothing to do
}

const std::set<CellPattern::Cell> &CellPattern::GetCells() const
{
    return *(this->activeCells);
}