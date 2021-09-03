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

#ifndef MANA_CHARACTER_HPP
#define MANA_CHARACTER_HPP

#include "engine/render/texturebuffer.hpp"

namespace mana {
    class Character {
    public:
        Character() = default;

        Character(TextureBuffer *texture, Vec2i size, Vec2i bearing, int advance)
                : texture(texture), size(size), bearing(bearing), advance(advance) {}

        ~Character() {
            delete texture;
        }

        Character(Character &&other) noexcept {
            texture = other.texture;
            size = other.size;
            bearing = other.bearing;
            advance = other.advance;

            other.texture = nullptr;
        }

        Character &operator=(Character &&other) noexcept {
            texture = other.texture;
            size = other.size;
            bearing = other.bearing;
            advance = other.advance;

            other.texture = nullptr;

            return *this;
        }

        Character(const Character &copy) = delete;

        Character &operator=(const Character &copy) = delete;

        TextureBuffer &getTexture() {
            return *texture;
        }

        /**
         * The size of the texture in pixels.
         *
         * @return
         */
        Vec2i getSize() const { return size; }

        /**
         * The bearing of the character in pixels.
         *
         * @return
         */
        Vec2i getBearing() const { return bearing; }

        /**
         * The advance of the character in pixels.
         *
         * @return
         */
        int getAdvance() const { return advance; }

    private:
        TextureBuffer *texture{};
        Vec2i size;
        Vec2i bearing;
        int advance{};
    };
}

#endif //MANA_CHARACTER_HPP
