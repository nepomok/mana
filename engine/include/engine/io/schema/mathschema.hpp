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

#ifndef MANA_MATHSCHEMA_HPP
#define MANA_MATHSCHEMA_HPP

#include "engine/io/message.hpp"

#include "common/grid.hpp"
#include "common/matrix.hpp"
#include "common/rectangle.hpp"
#include "common/transform.hpp"
#include "common/vector2.hpp"
#include "common/vector3.hpp"
#include "common/vector4.hpp"

namespace engine {
    Grid &operator<<(Grid &grid, const Message &message);

    Message &operator<<(Message &message, const Grid &grid);

    Mat4f &operator<<(Mat4f &matrix, const Message &message);

    Message &operator<<(Message &message, const Mat4f &grid);

    Rectf &operator<<(Rectf &mat, const Message &message);

    Message &operator<<(Message &message, const Rectf &grid);

    Transform &operator<<(Transform &grid, const Message &message);

    Message &operator<<(Message &message, const Transform &grid);

    Vec2f &operator<<(Vec2f &grid, const Message &message);

    Message &operator<<(Message &message, const Vec2f &grid);

    Vec2i &operator<<(Vec2i &grid, const Message &message);

    Message &operator<<(Message &message, const Vec2i &grid);

    Vec3f &operator<<(Vec3f &grid, const Message &message);

    Message &operator<<(Message &message, const Vec3f &grid);

    Vec4f &operator<<(Vec4f &grid, const Message &message);

    Message &operator<<(Message &message, const Vec4f &grid);

    Quaternion &operator<<(Quaternion &q, const Message &message);

    Message &operator<<(Message &message, const Quaternion &q);
}

#endif //MANA_MATHSCHEMA_HPP
