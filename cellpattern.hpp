#pragma once

#include <memory>
#include <set>
#include <utility>

class CellPattern
{
public:
    typedef std::pair<int16_t, int16_t> Cell;

    CellPattern();

    const std::set<Cell> &GetCells() const;

private:
    std::unique_ptr<std::set<Cell>> activeCells;
};