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

#ifndef MANA_FTFONTRASTERIZER_HPP
#define MANA_FTFONTRASTERIZER_HPP

#include <ft2build.h>
#include FT_FREETYPE_H

#include "engine/render/2d/text/fontrasterizer.hpp"

#include "ftfont.hpp"

namespace mana {
    class FTFontRasterizer : public FontRasterizer {
    public:
        explicit FTFontRasterizer(RenderAllocator &allocator);

        ~FTFontRasterizer() override;

        Font *createFont(std::string filePath) override;

        Font *createFont(std::istream &stream) override;

        Character rasterizeCharacter(Font &font, char c) override;

        std::map<char, Character> getAscii(Font &font) override;

    private:
        RenderAllocator &renderAllocator;

        FT_Library library;
    };
}

#endif //MANA_FTFONTRASTERIZER_HPP
