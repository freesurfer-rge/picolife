#pragma once

#include "sparselife/types.hpp"

#include <istream>
#include <memory>
#include <set>

namespace SparseLife
{
    class CellPattern
    {
    public:
        CellPattern();

        const std::set<Cell> &GetCells() const;

        void LoadFromStream(std::istream &is);

        void LoadRLEFromStream(std::istream &is);

        void ExchangeXY();
        void FlipX();
        void FlipY();

        void Translate(const int16_t dx, const int16_t dy);

    private:
        std::unique_ptr<std::set<Cell>> activeCells;

        bool IsComment(const std::string &s) const;
        bool RLEIgnoreLine(const std::string& s) const;
    };
}