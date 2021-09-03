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

#include <vector>
#include <iterator>
#include <istream>

#include <freetype/ftbitmap.h>

namespace mana {
    FTFontRasterizer::FTFontRasterizer(RenderAllocator &allocator)
            : renderAllocator(allocator), library() {
        if (FT_Init_FreeType(&library)) {
            throw std::runtime_error("Failed to init FreeType library");
        }
    }

    FTFontRasterizer::~FTFontRasterizer() {
        FT_Done_FreeType(library);
    }

    Font *FTFontRasterizer::createFont(std::string filePath) {
        auto *ret = new FTFont();
        auto r = FT_New_Face(library, filePath.c_str(), 0, &ret->face);
        if (r != 0) {
            throw std::runtime_error("Failed to create face from file at " + filePath + " " + std::to_string(r));
        }
        ret->setPixelSize(Vec2i(0, 50));
        return ret;
    }

    Font *FTFontRasterizer::createFont(std::istream &stream) {
        auto *ret = new FTFont();
        ret->bytes = std::vector<char>((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        auto r = FT_New_Memory_Face(library,
                                    reinterpret_cast<const FT_Byte *>(ret->bytes.data()),
                                    ret->bytes.size(),
                                    0,
                                    &ret->face);
        if (r != 0) {
            throw std::runtime_error("Failed to create face from memory " + std::to_string(r));
        }
        ret->setPixelSize(Vec2i(0, 100));
        return ret;
    }

    Character FTFontRasterizer::rasterizeCharacter(Font &font, char c) {
        auto &ftfont = dynamic_cast<FTFont &>(font);
        auto r = FT_Load_Char(ftfont.face, c, FT_LOAD_RENDER);
        if (r != 0) {
            throw std::runtime_error("Failed to rasterize character " + std::to_string(c) + " " + std::to_string(r));
        }

        Vec2i size(ftfont.face->glyph->bitmap.width, ftfont.face->glyph->bitmap.rows);
        Vec2i bearing(ftfont.face->glyph->bitmap_left, ftfont.face->glyph->bitmap_top);
        int advance = ftfont.face->glyph->advance.x >> 6;

        auto bitmap = ftfont.face->glyph->bitmap;
        auto pitch = bitmap.pitch;

        Image<unsigned char> buffer;
        if (pitch < 0) {
            //Descending
            auto rowLength = pitch * -1;
            if (bitmap.width != rowLength) {
                throw std::runtime_error("Invalid bitmap format");
            }
            buffer = Image<unsigned char>(static_cast<int>(bitmap.width), static_cast<int>(bitmap.rows));
            for (int x = 0; x < bitmap.width; x++) {
                for (int y = 0; y < bitmap.rows; y++) {
                    //Returns non random invalid pixels which together produce a pattern of circles instead of the rasterized character.
                    auto pixel = bitmap.buffer[bitmap.width * y + x];
                    buffer.setPixel(x, y, pixel);
                }
            }
        } else if (pitch > 0) {
            //Ascending
            auto rowLength = pitch;
            if (bitmap.width != rowLength) {
                throw std::runtime_error("Invalid bitmap format");
            }
            buffer = Image<unsigned char>(static_cast<int>(bitmap.width), static_cast<int>(bitmap.rows));
            for (int x = 0; x < bitmap.width; x++) {
                for (int y = 0; y < bitmap.rows; y++) {
                    //Returns non random invalid pixels which together produce a pattern of circles instead of the rasterized character.
                    auto pixel = bitmap.buffer[bitmap.width * y + x];
                    buffer.setPixel(x,  y, pixel);
                }
            }
        } else {
            buffer = Image<unsigned char>();
        }

        TextureBuffer::Attributes attribs;
        attribs.filterMin = TextureBuffer::NEAREST;
        attribs.filterMag = TextureBuffer::NEAREST;
        attribs.wrapping = TextureBuffer::CLAMP_TO_EDGE;

        TextureBuffer *texture = renderAllocator.createTextureBuffer(attribs);

        texture->upload(buffer);

        return std::move(Character(texture, size, bearing, advance));
    }

    std::map<char, Character> FTFontRasterizer::getAscii(Font &font) {
        std::map<char, Character> ret;
        for (int i = 0; i <= 127; i++) {
            ret[i] = std::move(rasterizeCharacter(font, static_cast<char>(i)));
        }
        return ret;
    }
}