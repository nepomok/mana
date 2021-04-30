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

#ifndef MANA_SHADERPROGRAM_HPP
#define MANA_SHADERPROGRAM_HPP

#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/matrix.hpp"

#include "engine/render/renderobject.hpp"

namespace mana {
    class ShaderProgram : public RenderObject {
    public:
        ~ShaderProgram() override = default;

        virtual void setBool(const std::string &name, bool value) = 0;

        virtual void setInt(const std::string &name, int value) = 0;

        virtual void setFloat(const std::string &name, float value) = 0;


        virtual void setVec2(const std::string &name, const Vec2b &value) = 0;

        virtual void setVec2(const std::string &name, const Vec2i &value) = 0;

        virtual void setVec2(const std::string &name, const Vec2f &value) = 0;


        virtual void setVec3(const std::string &name, const Vec3b &value) = 0;

        virtual void setVec3(const std::string &name, const Vec3i &value) = 0;

        virtual void setVec3(const std::string &name, const Vec3f &value) = 0;


        virtual void setVec4(const std::string &name, const Vec4b &value) = 0;

        virtual void setVec4(const std::string &name, const Vec4i &value) = 0;

        virtual void setVec4(const std::string &name, const Vec4f &value) = 0;


        virtual void setMat2(const std::string &name, const Mat2f &value) = 0;

        virtual void setMat3(const std::string &name, const Mat3f &value) = 0;

        virtual void setMat4(const std::string &name, const Mat4f &value) = 0;
    };
}

#endif //MANA_SHADERPROGRAM_HPP
