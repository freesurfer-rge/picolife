#include <cctype>
#include <limits>
#include <vector>

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

    void CellPattern::LoadRLEFromStream(std::istream &is)
    {
        bool reachedEnd = false;
        std::vector<std::string> tokens;

        std::string line;
        while (std::getline(is, line) && !reachedEnd)
        {
            bool reachedEnd = false;
            if (!this->RLEIgnoreLine(line))
            {
                std::string nextToken;
                for (auto c : line)
                {
                    if (!reachedEnd)
                    {
                        if (c == '!')
                        {
                            reachedEnd = true;
                            if (nextToken.size() > 0)
                            {
                                tokens.push_back(nextToken);
                                nextToken.clear();
                            }
                        }
                        if (std::isspace(c))
                        {
                            if (nextToken.size() != 0)
                            {
                                tokens.push_back(nextToken);
                                nextToken.clear();
                            }
                        }

                        if (std::isdigit(c))
                        {
                            nextToken.push_back(c);
                        }

                        if (c == 'b' || c == 'o' || c == '$')
                        {
                            // These characters terminate a token
                            nextToken.push_back(c);
                            tokens.push_back(nextToken);
                            nextToken.clear();
                        }
                    }
                }
            }
        }

        // Now, process the tokens into (count, tag) pairs
        std::vector<std::pair<uint16_t, char>> pairList;
        for (auto t : tokens)
        {
            if (t.size() == 1)
            {
                pairList.push_back(std::make_pair(1, t.at(0)));
            }
            else
            {
                const char tag = t.back();
                t.pop_back();
                uint16_t count = std::stoi(t);
                pairList.push_back(std::make_pair(count, tag));
            }
        }

        // Finally, process the pairs, inserting the cells
        uint16_t yCurr = 0;
        uint16_t xCurr = 0;
        for (auto p : pairList)
        {
            if (p.second == '$')
            {
                xCurr = 0;
                yCurr += p.first;
            }
            if (p.second == 'b')
            {
                // Dead cell
                xCurr += p.first;
            }
            if (p.second == 'o')
            {
                // Live cells
                for (uint16_t i = 0; i < p.first; ++i)
                {
                    auto nxtCell = Cell(xCurr, yCurr);
                    this->activeCells->emplace(nxtCell);
                    xCurr++;
                }
            }
        }
    }

    bool CellPattern::RLEIgnoreLine(const std::string &s) const
    {
        if (s.size() == 0)
        {
            return false;
        }

        bool ignore = false;
        ignore = ignore || s.at(0) == '#'; // Comment
        ignore = ignore || s.at(0) == 'x';

        return ignore;
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
            update.emplace(Cell(c.first + dx, c.second + dy));
        }
        this->activeCells->clear();
        this->activeCells->insert(update.begin(), update.end());
    }
}