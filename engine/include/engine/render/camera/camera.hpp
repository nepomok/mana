/**
 *  Mana - 3D Engine
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

#ifndef MANA_CAMERA_HPP
#define MANA_CAMERA_HPP

#include "engine/math/transform.hpp"
#include "engine/math/matrix.hpp"
#include "engine/math/matrixmath.hpp"

namespace mana {
    /**
     * A camera provides a view and projection matrix.
     */
    class Camera {
    public:
        virtual Mat4f view() const {
            Mat4f ret = MatrixMath::rotate(transform.rotation);
            //The engines move the universe (Negate camera position)
            return ret * MatrixMath::translate(transform.position * -1);
        }

        virtual Mat4f projection() const = 0;

        Transform transform;

        float nearClip = 0.1f;
        float farClip = 1000.0f;
    };
}

#endif //MANA_CAMERA_HPP
