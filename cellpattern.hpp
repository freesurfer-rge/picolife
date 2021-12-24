#pragma once

#include <istream>
#include <memory>
#include <set>
#include <string>
#include <utility>

class CellPattern
{
public:
    typedef std::pair<int16_t, int16_t> Cell;

    CellPattern();

    const std::set<Cell> &GetCells() const;

    void LoadFromStream(std::istream& is);
private:
    std::unique_ptr<std::set<Cell>> activeCells;

    bool IsComment(const std::string& s) const;
};