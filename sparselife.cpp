#include <map>

#include "sparselife.hpp"

SparseLife::SparseLife(const uint16_t xSize,
                       const uint16_t ySize,
                       const bool xWrap,
                       const bool yWrap) : nx(xSize),
                                           ny(ySize),
                                           wrapX(xWrap),
                                           wrapY(yWrap),
                                           activeCells(std::make_unique<std::set<Cell>>())
{
    // Nothing else to do
}

void SparseLife::AddCell(const SparseLife::Cell cell)
{
    this->activeCells->emplace(cell);
}

std::vector<SparseLife::Cell>
SparseLife::GetNeighbours(const SparseLife::Cell c) const
{
    std::vector<SparseLife::Cell> result;

    for (int16_t dx = -1; dx <= 1; ++dx)
    {
        int16_t xNeighbourPos = c.first + dx;
        if (this->wrapX)
        {
            xNeighbourPos = this->WrapValue(xNeighbourPos, this->nx);
        }
        for (int16_t dy = -1; dy <= 1; ++dy)
        {
            int16_t yNeighbourPos = c.second + dy;
            if (dx == 0 && dy == 0)
            {
                continue; // No self-neighbouring
            }

            if (this->wrapY)
            {
                yNeighbourPos = this->WrapValue(yNeighbourPos, this->ny);
            }

            // Check if neighbour is on the board
            if (xNeighbourPos >= 0 &&
                xNeighbourPos < this->nx &&
                yNeighbourPos >= 0 &&
                yNeighbourPos < this->ny)
            {
                result.push_back(std::make_pair(xNeighbourPos, yNeighbourPos));
            }
        }
    }

    return result;
}

int16_t SparseLife::WrapValue(const int16_t v, const uint16_t max) const
{
    if (v >= max)
    {
        return 0;
    }
    else if (v < 0)
    {
        return max - 1;
    }
    return v;
}

std::unique_ptr<std::set<SparseLife::Cell>>
SparseLife::ApplyRules(const std::set<SparseLife::Cell> &cellGrid) const
{
    std::map<Cell, uint8_t> neighbourCounts;

    // Find all the neighbour counts
    for (auto c : cellGrid)
    {
        auto neighbours = this->GetNeighbours(c);
        for (auto n : neighbours)
        {
            // Recall that a C++ map will auto initialise
            // a non-existent entry
            neighbourCounts[n] += 1;
        }
    }

    auto result = std::make_unique<std::set<Cell>>(cellGrid);

    // Apply the rules of Life
    for (auto nc : neighbourCounts)
    {
        // Kill off  things with wrong neighbour counts
        if (nc.second < 2 || nc.second > 3)
        {
            // Removing a nonexistent item appears harmless...
            result->erase(nc.first);
        }

        // Add new cells if three neighbours
        if (nc.second == 3)
        {
            result->emplace(nc.first);
        }
    }

    return result;
}

void SparseLife::Update()
{
    this->activeCells= this->ApplyRules(*(this->activeCells));
}