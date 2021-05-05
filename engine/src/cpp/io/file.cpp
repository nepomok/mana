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

#include "engine/io/file.hpp"

#include <iostream>
#include <cstring>

namespace mana {
    std::string File::readAllText(const std::string &filepath) {
        std::ifstream f(filepath);
        if (!f.is_open())
            throw std::runtime_error("Failed to open file at " + filepath + " " + strerror(errno));
        std::string ret;
        for (std::string tmp; std::getline(f, tmp);) {
            ret.append(tmp);
            ret.append("\n");
        }
        if (!f.eof()) {
            f.close();
            throw std::runtime_error("Failed to read file at " + filepath + " : " + strerror(errno));
        }
        f.close();
        return ret;
    }

    void File::writeAllText(const std::string &filepath, const std::string &text) {
        std::ofstream fos(filepath);
        if (!fos.is_open()) {
            throw std::runtime_error("Failed to open file at " + filepath + " " + strerror(errno));
        }
        fos << text;
        fos.close();
    }
}