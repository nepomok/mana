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

#ifndef MANA_FTFONT_HPP
#define MANA_FTFONT_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include "engine/render/2d/text/font.hpp"

namespace mana {
    class FTFont : public Font {
    public:
        FT_Face face{};
        std::vector<char> bytes; //Freetype requires the data to stay in memory when loading with FT_New_Memory_Face

        FTFont() = default;

        explicit FTFont(FT_Face face)
                : face(face) {}

        ~FTFont() override {
            FT_Done_Face(face);
        }

        void setPixelSize(Vec2i size) override {
            FT_Set_Pixel_Sizes(face, size.x, size.y);
        }
    };
}

#endif //MANA_FTFONT_HPP
