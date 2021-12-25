#include <limits>

#include "sparselife/cellpattern.hpp"

namespace SparseLife
{
    CellPattern::CellPattern() : activeCells(std::make_unique<std::set<Cell>>())
    {
        // Nothing to do
    }

    const std::set<Cell> &CellPattern::GetCells() const
    {
        return *(this->activeCells);
    }

    void CellPattern::LoadFromStream(std::istream &is)
    {
        std::string line;
        int16_t yCurr = 0;
        while (std::getline(is, line))
        {
            if (!this->IsComment(line))
            {
                for (size_t i = 0; i < line.size(); ++i)
                {
                    if (line.at(i) == 'O')
                    {
                        auto nxtCell = Cell(i, yCurr);
                        this->activeCells->emplace(nxtCell);
                    }
                }
                yCurr++;
            }
        }
    }

    bool CellPattern::IsComment(const std::string &s) const
    {
        if (s.size() == 0)
        {
            return false;
        }
        return s.at(0) == '!';
    }

    void CellPattern::ExchangeXY()
    {
        std::set<Cell> update;

        for (auto c : *(this->activeCells))
        {
            update.emplace(Cell(c.second, c.first));
        }

        this->activeCells->clear();
        this->activeCells->insert(update.begin(), update.end());
    }

    void CellPattern::FlipX()
    {
        std::set<Cell> update;
        int16_t xMax = std::numeric_limits<int16_t>::min();

        for (auto c : *(this->activeCells))
        {
            if (c.first > xMax)
            {
                xMax = c.first;
            }
            update.emplace(Cell(-c.first, c.second));
        }

        this->activeCells->clear();

        for (auto c : update)
        {
            this->activeCells->emplace(Cell(c.first + xMax, c.second));
        }
    }

    void CellPattern::FlipY()
    {
        std::set<Cell> update;
        int16_t yMax = std::numeric_limits<int16_t>::min();

        for (auto c : *(this->activeCells))
        {
            if (c.second > yMax)
            {
                yMax = c.second;
            }
            update.emplace(Cell(c.first, -c.second));
        }

        this->activeCells->clear();

        for (auto c : update)
        {
            this->activeCells->emplace(Cell(c.first, c.second + yMax));
        }
    }

    void CellPattern::Translate(const int16_t dx, const int16_t dy)
    {
        std::set<Cell> update;

        for (auto c : *(this->activeCells))
        {
            update.emplace(Cell(c.first + dx, c.second + dx));
        }
        this->activeCells->clear();
        this->activeCells->insert(update.begin(), update.end());
    }
}