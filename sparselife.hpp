#pragma once

#include <memory>
#include <set>
#include <utility>
#include <vector>

class SparseLife
{
public:
    // Adapted from
    // https://towardsdatascience.com/from-scratch-the-game-of-life-161430453ee3
    typedef std::pair<int16_t, int16_t> Cell;

    SparseLife(const uint16_t xSize, const uint16_t ySize, const bool xWrap, const bool yWrap);

    void AddCell(const Cell cell);
    void AddCells(const std::set<Cell>& cells);

    std::vector<Cell> GetNeighbours(const Cell c) const;
    std::unique_ptr<std::set<Cell>> ApplyRules(const std::set<Cell> &cellGrid) const;

    void Update();

    const std::set<Cell> &GetCells() const;

    const uint16_t nx;
    const uint16_t ny;
    const bool wrapX;
    const bool wrapY;

private:
    std::unique_ptr<std::set<Cell>> activeCells;

    int16_t WrapValue(const int16_t v, const uint16_t max) const;
};