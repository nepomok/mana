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

#include <stdexcept>

#include "engine/math/rotation.hpp"

#include "render/opengl/oglshaderprogram.hpp"
#include "render/opengl/ogltypeconverter.hpp"

#include "engine/math/matrixmath.hpp"

namespace mana {
    namespace opengl {
        OGLShaderProgram::OGLShaderProgram() : programID(0), vertexShader(), fragmentShader() {}

        OGLShaderProgram::OGLShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
                : vertexShader(vertexShader), fragmentShader(fragmentShader) {
            const std::string& vs = vertexShader;
            const std::string& fs = fragmentShader;

            const char *vertexSource = vs.c_str();
            const char *fragmentSource = fs.c_str();

            unsigned int vsH = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vsH, 1, &vertexSource, NULL);
            glCompileShader(vsH);
            int success;
            glGetShaderiv(vsH, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(vsH, 512, NULL, infoLog);
                glDeleteShader(vsH);
                std::string error = "Failed to compile vertex shader: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }

            unsigned int fsH = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fsH, 1, &fragmentSource, NULL);
            glCompileShader(fsH);
            glGetShaderiv(fsH, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(fsH, 512, NULL, infoLog);
                glDeleteShader(vsH);
                glDeleteShader(fsH);
                std::string error = "Failed to compile fragment shader: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }

            programID = glCreateProgram();
            glAttachShader(programID, vsH);
            glAttachShader(programID, fsH);
            glLinkProgram(programID);

            glDeleteShader(vsH);
            glDeleteShader(fsH);

            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(programID, 512, NULL, infoLog);
                std::string error = "Failed to compile shader program: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }
        }

        OGLShaderProgram::~OGLShaderProgram() {
            glDeleteProgram(programID);
        }

        void OGLShaderProgram::activate() {
            glUseProgram(programID);
        }

        void OGLShaderProgram::setBool(const std::string &name, bool value) {
            activate();
            glUniform1i(glGetUniformLocation(programID, name.c_str()), (int) value);
        }

        void OGLShaderProgram::setInt(const std::string &name, int value) {
            activate();
            glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
        }

        void OGLShaderProgram::setFloat(const std::string &name, float value) {
            activate();
            glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2b &value) {
            activate();
            glUniform2i(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2i &value) {
            activate();
            glUniform2i(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2f &value) {
            activate();
            glUniform2f(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3b &value) {
            activate();
            glUniform3i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3i &value) {
            activate();
            glUniform3i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3f &value) {
            activate();
            glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4b &value) {
            activate();
            glUniform4i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4i &value) {
            activate();
            glUniform4i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4f &value) {
            activate();
            glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setMat2(const std::string &name, const Mat2f &value) {
            throw std::runtime_error("Not Implemented");
        }

        void OGLShaderProgram::setMat3(const std::string &name, const Mat3f &value) {
            throw std::runtime_error("Not Implemented");
        }

        void OGLShaderProgram::setMat4(const std::string &name, const Mat4f &value) {
            activate();
            glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, (GLfloat *)&value);
        }
    }
}