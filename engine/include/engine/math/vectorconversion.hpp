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

#ifndef MANA_VECTORCONVERSION_HPP
#define MANA_VECTORCONVERSION_HPP

#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/vector4.hpp"

namespace mana {
    template<typename T>
    Vector2 <T> toVec2(const Vector3 <T> &vec) { return {vec.x, vec.y}; }

    Vec2b toVec2(const Vec3b &vec) { return toVec2<bool>(vec); }

    Vec2i toVec2(const Vec3i &vec) { return toVec2<int>(vec); }

    Vec2f toVec2(const Vec3f &vec) { return toVec2<float>(vec); }

    Vec2d toVec2(const Vec3d &vec) { return toVec2<double>(vec); }

    template<typename T>
    Vector2 <T> toVec2(const Vector4 <T> &vec) { return {vec.x, vec.y}; }

    Vec2b toVec2(const Vec4b &vec) { return toVec2<bool>(vec); }

    Vec2i toVec2(const Vec4i &vec) { return toVec2<int>(vec); }

    Vec2f toVec2(const Vec4f &vec) { return toVec2<float>(vec); }

    Vec2d toVec2(const Vec4d &vec) { return toVec2<double>(vec); }

    template<typename T>
    Vector3 <T> toVec3(const Vector2 <T> &vec, T z) { return {vec.x, vec.y, z}; }

    Vec3b toVec3(const Vec2b &vec, bool z = false) { return toVec3<bool>(vec, z); }

    Vec3i toVec3(const Vec2i &vec, int z = 0) { return toVec3<int>(vec, z); }

    Vec3f toVec3(const Vec2f &vec, float z = 0) { return toVec3<float>(vec, z); }

    Vec3d toVec3(const Vec2d &vec, double z = 0) { return toVec3<double>(vec, z); }

    template<typename T>
    Vector3 <T> toVec3(const Vector4 <T> &vec) { return {vec.x, vec.y, vec.z}; }

    Vec3b toVec3(const Vec4b &vec) { return toVec3<bool>(vec); }

    Vec3i toVec3(const Vec4i &vec) { return toVec3<int>(vec); }

    Vec3f toVec3(const Vec4f &vec) { return toVec3<float>(vec); }

    Vec3d toVec3(const Vec4d &vec) { return toVec3<double>(vec); }

    template<typename T>
    Vector4 <T> toVec4(const Vector2 <T> &vec, T z, T w) { return {vec.x, vec.y, z, w}; }

    Vec4b toVec4(const Vec2b &vec, bool z = false, bool w = false) { return toVec4<bool>(vec, z, w); }

    Vec4i toVec4(const Vec2i &vec, int z = false, int w = false) { return toVec4<int>(vec, z, w); }

    Vec4f toVec4(const Vec2f &vec, float z = false, float w = false) { return toVec4<float>(vec, z, w); }

    Vec4d toVec4(const Vec2d &vec, double z = false, double w = false) { return toVec4<double>(vec, z, w); }

    template<typename T>
    Vector4 <T> toVec4(const Vector3 <T> &vec, T w) { return {vec.x, vec.y, vec.z, w}; }

    Vec4b toVec4(const Vec3b &vec, bool w = false) { return toVec4<bool>(vec, w); }

    Vec4i toVec4(const Vec3i &vec, int w = 0) { return toVec4<int>(vec, w); }

    Vec4f toVec4(const Vec3f &vec, float w = 0) { return toVec4<float>(vec, w); }

    Vec4d toVec4(const Vec3d &vec, double w = 0) { return toVec4<double>(vec, w); }
}

#endif //MANA_VECTORCONVERSION_HPP
