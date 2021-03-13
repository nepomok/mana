#ifndef MANA_OGLUSERFRAMEBUFFER_HPP
#define MANA_OGLUSERFRAMEBUFFER_HPP

#include "render/opengl/oglframebufferobject.hpp"

namespace mana {
    namespace opengl {
        class OGLUserFrameBuffer : public OGLFrameBufferObject {
        public:
            GLuint FBO;

            int width;
            int height;

            GLuint colorBuffer;
            GLuint renderBuffer;

            bool userTexture = false;

            OGLUserFrameBuffer() : FBO(0),
                                   width(0),
                                   height(0),
                                   colorBuffer(0),
                                   renderBuffer(0),
                                   userTexture(false) {}

            OGLUserFrameBuffer(int width, int height)
                    : width(width),
                      height(height),
                      FBO(0),
                      colorBuffer(0),
                      renderBuffer(0),
                      userTexture(false) {}

            ~OGLUserFrameBuffer() override {
                glDeleteRenderbuffers(1, &renderBuffer);
                if (!userTexture) {
                    glDeleteTextures(1, &colorBuffer);
                }
                glDeleteFramebuffers(1, &FBO);
            }

            Vec2i getSize() const override {
                return {width, height};
            }

            GLuint getFBO() const override {
                return FBO;
            }
        };
    }
}

#endif //MANA_OGLUSERFRAMEBUFFER_HPP
