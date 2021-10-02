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

#include <stdexcept>

#include "engine/math/rotation.hpp"

#include "engine/render/shadercompiler.hpp"

#include "engine/math/matrixmath.hpp"

#include "oglshaderprogram.hpp"
#include "oglcheckerror.hpp"

#define PREFIX_GLOBAL "Globals."

namespace engine {
    namespace opengl {
        OGLShaderProgram::OGLShaderProgram() : programID(0) {}

        OGLShaderProgram::OGLShaderProgram(const std::string &vertexShader,
                                           const std::string &geometryShader,
                                           const std::string &fragmentShader)
                : programID(0) {
            const char *vertexSource = vertexShader.c_str();
            const char *geometrySource = geometryShader.c_str();
            const char *fragmentSource = fragmentShader.c_str();

            programID = glCreateProgram();


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

            glAttachShader(programID, vsH);

            unsigned int gsH;

            if (!geometryShader.empty()) {
                gsH = glCreateShader(GL_GEOMETRY_SHADER);
                glShaderSource(gsH, 1, &geometrySource, NULL);
                glCompileShader(gsH);
                glGetShaderiv(gsH, GL_COMPILE_STATUS, &success);
                if (!success) {
                    char infoLog[512];
                    glGetShaderInfoLog(gsH, 512, NULL, infoLog);
                    glDeleteShader(vsH);
                    glDeleteShader(gsH);
                    std::string error = "Failed to compile fragment shader: ";
                    error.append(infoLog);
                    throw std::runtime_error(error);
                }
                glAttachShader(programID, gsH);
            }

            unsigned int fsH = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fsH, 1, &fragmentSource, NULL);
            glCompileShader(fsH);
            glGetShaderiv(fsH, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(fsH, 512, NULL, infoLog);
                glDeleteShader(vsH);
                if (!geometryShader.empty())
                    glDeleteShader(gsH);
                glDeleteShader(fsH);
                std::string error = "Failed to compile fragment shader: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }
            glAttachShader(programID, fsH);

            glLinkProgram(programID);

            glDeleteShader(vsH);
            glDeleteShader(fsH);

            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(programID, 512, NULL, infoLog);
                std::string error = "Failed to link shader program: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }

            checkGLError("");
        }

        OGLShaderProgram::~OGLShaderProgram() {
            glDeleteProgram(programID);
        }

        void OGLShaderProgram::activate() const {
            glUseProgram(programID);
            checkGLError("");
        }

        bool OGLShaderProgram::setTexture(const std::string &name, int slot) {
            activate();
            GLuint i = glGetUniformLocation(programID, name.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform1i(i, (int) slot);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setBool(const std::string &name, bool value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform1i(i, (int) value);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setInt(const std::string &name, int value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform1i(i, value);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setFloat(const std::string &name, float value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform1f(i, value);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec2(const std::string &name, const Vec2b &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform2i(i, value.x, value.y);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec2(const std::string &name, const Vec2i &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform2i(i, value.x, value.y);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec2(const std::string &name, const Vec2f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform2f(i, value.x, value.y);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec3(const std::string &name, const Vec3b &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform3i(i, value.x, value.y, value.z);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec3(const std::string &name, const Vec3i &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform3i(i, value.x, value.y, value.z);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec3(const std::string &name, const Vec3f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform3f(i, value.x, value.y, value.z);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec4(const std::string &name, const Vec4b &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform4i(i, value.x, value.y, value.z, value.w);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec4(const std::string &name, const Vec4i &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform4i(i, value.x, value.y, value.z, value.w);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setVec4(const std::string &name, const Vec4f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniform4f(i, value.x, value.y, value.z, value.w);
            }
            checkGLError("");
            return ret;
        }

        bool OGLShaderProgram::setMat2(const std::string &name, const Mat2f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            throw std::runtime_error("Not Implemented");
        }

        bool OGLShaderProgram::setMat3(const std::string &name, const Mat3f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            throw std::runtime_error("Not Implemented");
        }

        bool OGLShaderProgram::setMat4(const std::string &name, const Mat4f &value) {
            std::string globName = PREFIX_GLOBAL + name;
            activate();
            GLuint i = glGetUniformLocation(programID, globName.c_str());
            bool ret = false;
            if (i != -1) {
                ret = true;
                glUniformMatrix4fv(i, 1, GL_FALSE, (GLfloat *) &value);
            }
            checkGLError("");
            return ret;
        }
    }
}