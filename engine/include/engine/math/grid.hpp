/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GRID_HPP
#define GRID_HPP

#include <cassert>
#include <vector>

#include "engine/math/rectangle.hpp"
#include "engine/math/vector2.hpp"

namespace engine {
    class Grid {
    public:
        int nCol, nRow;
        Vec2i size;

        Grid(int nColumns, int nRows, Vec2i size)
                : nCol(nColumns), nRow(nRows), size(size) {
            assert(nColumns > 0);
            assert(nRows > 0);
            assert(size.x > 0);
            assert(size.y > 0);
            assert(size.x % nColumns == 0);
            assert(size.y % nRows == 0);
        }

        Grid()
                : nCol(0), nRow(0), size(0) {
        }

        Vec2i getCellSize() const {
            return {size.x / nCol, size.y / nRow};
        }

        Recti getCell(int column, int row) const {
            assert(column < nCol);
            assert(row < nRow);

            return {Vec2i(column * (size.x / nCol), row * (size.y / nRow)),
                    Vec2i(size.x / nCol, size.y / nRow)};
        }

        std::vector<Recti> getColumn(int column, int numberOfRows = 0) const {
            assert(column < nCol);
            assert(numberOfRows <= nRow);

            if (numberOfRows <= 0)
                numberOfRows = nRow;
            std::vector<Recti> ret;
            for (int row = 0; row < numberOfRows; row++) {
                ret.push_back(getCell(column, row));
            }
            return ret;
        }

        std::vector<Recti> getRow(int row, int numberOfColumns = 0) const {
            assert(row < nRow);
            assert(numberOfColumns <= nCol);

            if (numberOfColumns <= 0)
                numberOfColumns = nCol;
            std::vector<Recti> ret;
            for (int col = 0; col < numberOfColumns; col++) {
                ret.push_back(getCell(col, row));
            }
            return ret;
        }
    };
}

#endif //GRID_HPP