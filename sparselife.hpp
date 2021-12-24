#pragma once

#include <map>
#include <set>
#include <utility>

class SparseLife
{
public:
    typedef std::pair<int16_t, int16_t> Cell;

    SparseLife(const uint16_t xSize, const uint16_t ySize, const bool xWrap, const bool yWrap);

    std::set<Cell> GetNeighbours(const Cell c);

    const uint16_t nx;
    const uint16_t ny;
    const bool wrapX;
    const bool wrapY;

private:
};