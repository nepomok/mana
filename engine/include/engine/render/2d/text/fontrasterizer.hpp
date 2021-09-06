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

#ifndef MANA_FONTRASTERIZER_HPP
#define MANA_FONTRASTERIZER_HPP

#include "engine/render/2d/text/character.hpp"
#include "engine/render/2d/text/font.hpp"

#include "engine/render/renderdevice.hpp"

namespace engine {
    class FontRasterizer {
    public:
        enum Backend {
            FreeType
        };

        /**
         * To avoid exposing library headers to users we will also put the font rasterization api behind an interface.
         *
         * @param backend
         * @param allocator
         * @return
         */
        static FontRasterizer *instantiate(Backend backend, RenderAllocator &allocator);

        virtual ~FontRasterizer() = default;

        virtual Font *createFont(std::string filePath) = 0;

        virtual Font *createFont(std::istream &stream) = 0;

        /**
         *  Rasterize the given ascii character using the given font.
         *
         * @param font
         * @param c
         * @return
         */
        virtual Character rasterizeCharacter(Font &font, char c) = 0;

        /**
         * Convenience method which rasterizes all ascii characters and returns the character mapping.
         *
         * @param font
         * @return
         */
        virtual std::map<char, Character> getAscii(Font &font) = 0;

        //TODO: Unicode character rasterization
    };
}
#endif //MANA_FONTRASTERIZER_HPP
