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

#ifndef MANA_QtOGLSHADERPROGRAM_HPP
#define MANA_QtOGLSHADERPROGRAM_HPP

#include <string>

#include "engine/render/shaderprogram.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/matrix.hpp"

#include "openglinclude.hpp"

#include <QOpenGLFunctions_3_3_Core>

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
        class QtOGLShaderProgram : public ShaderProgram, public QOpenGLFunctions_3_3_Core {
        public:
            QtOGLShaderProgram();

            QtOGLShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

            ~QtOGLShaderProgram() override;

            QtOGLShaderProgram(const QtOGLShaderProgram &copy) = delete;

            QtOGLShaderProgram &operator=(const QtOGLShaderProgram &) = delete;

            void activate();

            bool setTexture(const std::string &name, int slot) override;

            bool setBool(const std::string &name, bool value) override;

            bool setInt(const std::string &name, int value) override;

            bool setFloat(const std::string &name, float value) override;

            bool setVec2(const std::string &name, const Vec2b &value) override;

            bool setVec2(const std::string &name, const Vec2i &value) override;

            bool setVec2(const std::string &name, const Vec2f &value) override;

            bool setVec3(const std::string &name, const Vec3b &value) override;

            bool setVec3(const std::string &name, const Vec3i &value) override;

            bool setVec3(const std::string &name, const Vec3f &value) override;

            bool setVec4(const std::string &name, const Vec4b &value) override;

            bool setVec4(const std::string &name, const Vec4i &value) override;

            bool setVec4(const std::string &name, const Vec4f &value) override;

            bool setMat2(const std::string &name, const Mat2f &value) override;

            bool setMat3(const std::string &name, const Mat3f &value) override;

            bool setMat4(const std::string &name, const Mat4f &value) override;

        private:
            GLuint programID;

            std::string vertexShader;
            std::string fragmentShader;
        };
    }
}

#endif //MANA_QtOGLSHADERPROGRAM_HPP
