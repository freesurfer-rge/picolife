#pragma once

#include <memory>
#include <utility>

class CellPattern
{
public:
    typedef std::pair<int16_t, int16_t> Cell;

    const std::set<Cell> &GetCells() const;

private:
    std::unique_ptr<std::set<Cell>> activeCells;
};