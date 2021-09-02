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

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "engine/math/vector2.hpp"

namespace mana {
    /**
     * A axis aligned rectangle defining position and dimensions.
     * */
    template<typename T>
    struct Rectangle {
        Vector2 <T> position; //Top left point of the rectangle
        Vector2 <T> dimensions;

        Rectangle()
                : position(0), dimensions(0) {
        }

        Rectangle(Vector2 <T> position, Vector2 <T> dimensions)
                : position(position), dimensions(dimensions) {
        }

        Vector2 <T> center() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y - dimensions.y / 2);
        }

        Vector2 <T> top() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y);
        }

        Vector2 <T> right() {
            return Vector2<T>(position.x + dimensions.x, position.y - dimensions.y / 2);
        }

        Vector2 <T> bottom() {
            return Vector2<T>(position.x + dimensions.x / 2, position.y - dimensions.y);
        }

        Vector2 <T> left() {
            return Vector2<T>(position.x, position.y - dimensions.y / 2);
        }

        template<typename R>
        Rectangle<R> convert() const {
            return Rectangle<R>(Vector2<R>(static_cast<R>(position.x), static_cast<R>(position.y)),
                                Vector2<R>(static_cast<R>(dimensions.x), static_cast<R>(dimensions.y)));
        }

        bool testPoint(Vector2 <T> point) {
            return point.x >= position.x && point.y <= position.y && point.x <= position.x + dimensions.x &&
                   point.y >= position.y - dimensions.y;
        }

        bool testOverlap(Rectangle<T> other) {
            if (position.x > other.position.x + other.dimensions.x)
                return false;
            else if (position.y < other.position.y - other.dimensions.y)
                return false;
            else if (position.x + dimensions.x < other.position.x)
                return false;
            else if (position.y - dimensions.y > other.position.y)
                return false;
            else
                return true;
        }
    };

    typedef Rectangle<float> Rectf;
    typedef Rectangle<int> Recti;
}

#endif // RECTANGLE_HPP