#include <limits>

#include "cellpattern.hpp"

CellPattern::CellPattern() : activeCells(std::make_unique<std::set<CellPattern::Cell>>())
{
    // Nothing to do
}

const std::set<CellPattern::Cell> &CellPattern::GetCells() const
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
    std::set<CellPattern::Cell> update;

    for (auto c : *(this->activeCells))
    {
        update.emplace(Cell(c.second, c.first));
    }

    this->activeCells->clear();

    for (auto c : update)
    {
        this->activeCells->emplace(c);
    }
}

void CellPattern::FlipX()
{
    std::set<CellPattern::Cell> update;
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