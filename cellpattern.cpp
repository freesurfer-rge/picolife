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
        if( !this->IsComment(line))
        {
            for( size_t i=0; i<line.size(); ++i)
            {
                if( line.at(i)=='O')
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