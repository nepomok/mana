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

#ifndef ROTATION_HPP
#define ROTATION_HPP

#include <cmath>

#include "engine/math/vector2.hpp"

#define PI 3.141592653589793238463

namespace engine {
    inline double radiansToDegrees(double radians) {
        return radians * (180.0 / PI);
    }

    inline double degreesToRadians(double degrees) {
        return degrees * PI / 180.0;
    }

    inline float radiansToDegrees(float radians) {
        return (float) (radians * (180.0 / PI));
    }

    inline float degreesToRadians(float degrees) {
        return (float) (degrees * PI / 180.0);
    }

    //Rotate the vector around the point clockwise by angleDegrees
    inline Vec2f rotateVectorAroundPoint(Vec2f vector, Vec2f point, double angleDegrees) {
        double radians = degreesToRadians(angleDegrees);

        //https://math.stackexchange.com/questions/814950/how-can-i-rotate-a-coordinate-around-a-circle
        Vec2f ret;
        ret.x = std::cos(radians) * (vector.x - point.x) - std::sin(radians) * (vector.y - point.y) + point.x;
        ret.y = std::sin(radians) * (vector.x - point.x) + std::cos(radians) * (vector.y - point.y) + point.y;
        return ret;
    }
}

#endif // ROTATION_HPP