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

#include "engine/math/grid.hpp"
#include "engine/math/matrix.hpp"
#include "engine/math/rectangle.hpp"
#include "engine/math/transform.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/vector4.hpp"

namespace engine::runtime {
    Grid &operator<<(Grid &grid, const Message &message);

    Message &operator<<(Message &message, const Grid &grid);

    template<typename T, int W, int H>
    Matrix<T, W, H> &operator<<(Matrix<T, W, H> &matrix, const Message &message);

    template<typename T, int W, int H>
    Message &operator<<(Message &message, const Matrix<T, W, H> &grid);

    template<typename T>
    Rectangle<T> &operator<<(Rectangle<T> &mat, const Message &message);

    template<typename T>
    Message &operator<<(Message &message, const Rectangle<T> &grid);

    Transform &operator<<(Transform &grid, const Message &message);

    Message &operator<<(Message &message, const Transform &grid);

    template<typename T>
    Vector2<T> &operator<<(Vector2<T> &grid, const Message &message);

    template<typename T>
    Message &operator<<(Message &message, const Vector2<T> &grid);

    template<typename T>
    Vector3<T> &operator<<(Vector3<T> &grid, const Message &message);

    template<typename T>
    Message &operator<<(Message &message, const Vector3<T> &grid);

    template<typename T>
    Vector4<T> &operator<<(Vector4<T> &grid, const Message &message);

    template<typename T>
    Message &operator<<(Message &message, const Vector4<T> &grid);
}

#endif //MANA_MATHSCHEMA_HPP
