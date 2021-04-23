#ifndef MANA_OGLSHADERPROGRAM_HPP
#define MANA_OGLSHADERPROGRAM_HPP

#include <string>

#include "engine/render/shaderprogram.hpp"
#include "engine/math/vector2.hpp"
#include "engine/math/vector3.hpp"
#include "engine/math/matrix.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        /**
         * The OGLShaderProgram implementation features a custom preprocessor which processes #include directives.
         *
         * Users may use the #include symbol in the passed shader source to instruct the preprocessor to inject some glsl.
         *
         * The directive string is then replaced by the custom preprocessor before passing the shader to the glfw compiler.
         *
         * The following tokens are valid for a #include directive:
         * #include "mana.glsl"      ---     For now everything (Matrices, Lighting etc.)
         *
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

            OGLShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);

            ~OGLShaderProgram() override;

            OGLShaderProgram(const OGLShaderProgram &copy) = delete;

            OGLShaderProgram &operator=(const OGLShaderProgram &) = delete;

            void activate();

            void setBool(const std::string &name, bool value) override;

            void setInt(const std::string &name, int value) override;

            void setFloat(const std::string &name, float value) override;

            void setVec2(const std::string &name, const Vec2b &value) override;

            void setVec2(const std::string &name, const Vec2i &value) override;

            void setVec2(const std::string &name, const Vec2f &value) override;

            void setVec3(const std::string &name, const Vec3b &value) override;

            void setVec3(const std::string &name, const Vec3i &value) override;

            void setVec3(const std::string &name, const Vec3f &value) override;

            void setVec4(const std::string &name, const Vec4b &value) override;

            void setVec4(const std::string &name, const Vec4i &value) override;

            void setVec4(const std::string &name, const Vec4f &value) override;

            void setMat2(const std::string &name, const Mat2f &value) override;

            void setMat3(const std::string &name, const Mat3f &value) override;

            void setMat4(const std::string &name, const Mat4f &value) override;

        private:
            GLuint programID;

            std::string vertexShader;
            std::string fragmentShader;
        };
    }
}

#endif //MANA_OGLSHADERPROGRAM_HPP
