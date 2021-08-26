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

#include "render/2d/text/freetype/ftfontrasterizer.hpp"

namespace mana {
    FTFontRasterizer::FTFontRasterizer(RenderAllocator &allocator)
            : renderAllocator(allocator), library() {
        if (FT_Init_FreeType(&library)) {
            throw std::runtime_error("Failed to init FreeType library");
        }
    }

    FTFontRasterizer::~FTFontRasterizer() = default;

    Font *FTFontRasterizer::createFont(std::string filePath) {
        auto *ret = new FTFont();
        if (FT_New_Face(library, filePath.c_str(), 0, &ret->face)) {
            throw std::runtime_error("Failed to create face from file at " + filePath);
        }
        return ret;
    }

    Font *FTFontRasterizer::createFont(std::istream &stream) {
        //TODO: Implement font creation from c++ stream.
        throw std::runtime_error("Not Implemented");
    }

    Character FTFontRasterizer::rasterizeCharacter(Font &font, char c) {
        auto &ftfont = dynamic_cast<FTFont &>(font);
        if (FT_Load_Char(ftfont.face, c, FT_LOAD_RENDER)) {
            throw std::runtime_error("Failed to rasterize character " + std::to_string(c));
        }

        //TODO:PERF: Two copies of the freetype bitmap occur here. Refactor ImageBuffer api for more efficient low level access.
        Vec2i size(ftfont.face->glyph->bitmap.width, ftfont.face->glyph->bitmap.rows);
        Vec2i bearing(ftfont.face->glyph->bitmap_left, ftfont.face->glyph->bitmap_top);
        int advance = ftfont.face->glyph->advance.x;

        ImageBuffer<unsigned char> buffer(size.x,
                                          size.y,
                                          std::vector<unsigned char>(ftfont.face->glyph->bitmap.buffer,
                                                                     ftfont.face->glyph->bitmap.buffer +
                                                                     size.x * size.y));
        TextureBuffer::Attributes attribs;
        attribs.format = TextureBuffer::R;

        TextureBuffer *texture = renderAllocator.createTextureBuffer(attribs);

        return Character(texture, size, bearing, advance);
    }

    std::map<char, Character> FTFontRasterizer::getAscii(Font &font) {
        std::map<char, Character> ret;
        for (char i = 0; i <= 127; i++) {
            ret[i] = rasterizeCharacter(font, i);
        }
        return ret;
    }
}