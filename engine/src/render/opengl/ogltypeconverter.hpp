#ifndef MANA_OGLTYPECONVERTER_HPP
#define MANA_OGLTYPECONVERTER_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "engine/math/vector3.hpp"
#include "engine/render/textureattributes.hpp"
#include "engine/render/renderunit.hpp"

namespace mana {
    namespace opengl {
        class OGLTypeConverter {
        public:
            static GLint convertPrimitive(int value) {
                return (GLint) value;
            }

            static GLuint convertPrimitive(uint value) {
                return (GLuint) value;
            }

            static glm::vec3 convert(const Vec3f &vec) {
                return glm::vec3(vec.x, vec.y, vec.z);
            }

            static Vec3f convert(const glm::vec3 &vec) {
                return {vec.x, vec.y, vec.z};
            }

            static glm::mat4 convert(const Mat4f &mat) {
                glm::mat4 ret;
                for (int x = 0; x < mat.width(); x++) {
                    for (int y = 0; y < mat.height(); y++) {
                        ret[x][y] = mat.get(x, y);
                    }
                }
                return ret;
            }

            static Mat4f convert(const glm::mat4 &mat) {
                Mat4f ret;
                for (int x = 0; x < 4; x++) {
                    for (int y = 0; y < 4; y++) {
                        ret.set(x, y, mat[x][y]);
                    }
                }
                return ret;
            }

            static GLint convert(TextureWrapping wrapping) {
                switch (wrapping) {
                    case REPEAT:
                        return GL_REPEAT;
                    case MIRRORED_REPEAT:
                        return GL_MIRRORED_REPEAT;
                    case CLAMP_TO_EDGE:
                        return GL_CLAMP_TO_EDGE;
                    case CLAMP_TO_BORDER:
                        return GL_CLAMP_TO_BORDER;
                }
                throw std::runtime_error("Invalid texture wrapping");
            }

            static GLint convert(TextureFiltering filtering) {
                switch (filtering) {
                    case NEAREST:
                        return GL_NEAREST;
                    case LINEAR:
                        return GL_LINEAR;
                }
                throw std::runtime_error("Invalid texture filtering");
            }

            static GLint convert(MipMapFiltering filtering) {
                switch (filtering) {
                    case NEAREST_MIPMAP_NEAREST:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case LINEAR_MIPMAP_NEAREST:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case NEAREST_MIPMAP_LINEAR:
                        return GL_NEAREST_MIPMAP_LINEAR;
                    case LINEAR_MIPMAP_LINEAR:
                        return GL_LINEAR_MIPMAP_LINEAR;
                }
                throw std::runtime_error("Invalid mipmap filtering");
            }

            static GLenum convert(FaceCullingMode mode) {
                switch (mode) {
                    case CULL_NONE:
                        return GL_NONE;
                    case CULL_FRONT:
                        return GL_FRONT;
                    case CULL_BACK:
                        return GL_BACK;
                }
                throw std::runtime_error("Unsupported culling mode");
            }

            static GLenum convert(const DepthTestMode &mode) {
                switch (mode) {
                    case DEPTH_TEST_ALWAYS:
                        return GL_ALWAYS;
                    case DEPTH_TEST_NEVER:
                        return GL_NEVER;
                    case DEPTH_TEST_LESS:
                        return GL_LESS;
                    case DEPTH_TEST_EQUAL:
                        return GL_EQUAL;
                    case DEPTH_TEST_LEQUAL:
                        return GL_LEQUAL;
                    case DEPTH_TEST_GREATER:
                        return GL_GREATER;
                    case DEPTH_TEST_NOTEQUAL:
                        return GL_NOTEQUAL;
                    case DEPTH_TEST_GEQUAL:
                        return GL_GEQUAL;
                }
                throw std::runtime_error("Unsupported depth testing mode");
            }

            static GLenum convert(BlendMode mode) {
                switch (mode) {
                    case ZERO:
                        return GL_ZERO;
                    case ONE:
                        return GL_ONE;
                    case SRC_COLOR:
                        return GL_SRC_COLOR;
                    case ONE_MINUS_SRC_COLOR:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case DST_COLOR:
                        return GL_DST_COLOR;
                    case SRC_ALPHA:
                        return GL_SRC_ALPHA;
                    case ONE_MINUS_SRC_ALPHA:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case DST_ALPHA:
                        return GL_DST_ALPHA;
                    case ONE_MINUS_DST_ALPHA:
                        return GL_ONE_MINUS_DST_ALPHA;
                    case CONSTANT_COLOR:
                        return GL_CONSTANT_COLOR;
                    case ONE_MINUS_CONSTANT_COLOR:
                        return GL_ONE_MINUS_CONSTANT_COLOR;
                    case CONSTANT_ALPHA:
                        return GL_CONSTANT_ALPHA;
                    case ONE_MINUS_CONSTANT_ALPHA:
                        return GL_ONE_MINUS_CONSTANT_ALPHA;
                }
                throw std::runtime_error("Unsupported blending mode");
            }

            static GLenum convert(StencilMode mode) {
                switch (mode) {
                    case STENCIL_NEVER:
                        return GL_NEVER;
                    case STENCIL_LESS:
                        return GL_LESS;
                    case STENCIL_LEQUAL:
                        return GL_LEQUAL;
                    case STENCIL_GREATER:
                        return GL_GREATER;
                    case STENCIL_GEQUAL:
                        return GL_GEQUAL;
                    case STENCIL_EQUAL:
                        return GL_EQUAL;
                    case STENCIL_NOTEQUAL:
                        return GL_NOTEQUAL;
                    case STENCIL_ALWAYS:
                        return GL_ALWAYS;
                }
                throw std::runtime_error("Unsupported stencil mode");
            }

            static GLenum convert(StencilAction action) {
                switch (action) {
                    case STENCIL_KEEP:
                        return GL_KEEP;
                    case STENCIL_ZERO:
                        return GL_ZERO;
                    case STENCIL_REPLACE:
                        return GL_REPLACE;
                    case STENCIL_INCR:
                        return GL_INCR;
                    case STENCIL_INCR_WRAP:
                        return GL_INCR_WRAP;
                    case STENCIL_DECR:
                        return GL_DECR;
                    case STENCIL_DECR_WRAP:
                        return GL_DECR_WRAP;
                    case STENCIL_INVERT:
                        return GL_INVERT;
                }
                throw std::runtime_error("Unsupported stencil action");
            }
        };
    }
}

#endif //MANA_OGLTYPECONVERTER_HPP
