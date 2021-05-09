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

#ifndef MANA_OGLSHADERPROGRAM_HPP
#define MANA_OGLSHADERPROGRAM_HPP

#include <string>
#include <map>
#include <functional>

#include "engine/render/shaderprogram.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/matrix.hpp"

#include "openglinclude.hpp"

namespace mana {
    namespace opengl {
        /**
         * The following vertex shader input layout is required:
         *
         * layout (location = 0) in vec3 position;
         * layout (location = 1) in vec3 normal;
         * layout (location = 2) in vec2 uv;
         * layout (location = 3) in mat4 instanceMatrix;
         */
        class OGLShaderProgram : public ShaderProgram {
        public:
            OGLShaderProgram();

            OGLShaderProgram(const std::string &vertexShader, const std::string &fragmentShader,
                             const std::map<std::string, std::string> &macros,
                             const std::function<std::string(const char *)> &includeCallback);

            ~OGLShaderProgram() override;

            OGLShaderProgram(const OGLShaderProgram &copy) = delete;

            OGLShaderProgram &operator=(const OGLShaderProgram &) = delete;

            void activate() const;

            bool setTexture(const std::string &name, int slot) const override;

            bool setBool(const std::string &name, bool value) const override;

            bool setInt(const std::string &name, int value) const override;

            bool setFloat(const std::string &name, float value) const override;

            bool setVec2(const std::string &name, const Vec2b &value) const override;

            bool setVec2(const std::string &name, const Vec2i &value) const override;

            bool setVec2(const std::string &name, const Vec2f &value) const override;

            bool setVec3(const std::string &name, const Vec3b &value) const override;

            bool setVec3(const std::string &name, const Vec3i &value) const override;

            bool setVec3(const std::string &name, const Vec3f &value) const override;

            bool setVec4(const std::string &name, const Vec4b &value) const override;

            bool setVec4(const std::string &name, const Vec4i &value) const override;

            bool setVec4(const std::string &name, const Vec4f &value) const override;

            bool setMat2(const std::string &name, const Mat2f &value) const override;

            bool setMat3(const std::string &name, const Mat3f &value) const override;

            bool setMat4(const std::string &name, const Mat4f &value) const override;

        private:
            GLuint programID;

            std::string vertexShader;
            std::string fragmentShader;
        };
    }
}

#endif //MANA_OGLSHADERPROGRAM_HPP
