#ifndef MANA_OGLUSERFRAMEBUFFEROBJECT_HPP
#define MANA_OGLUSERFRAMEBUFFEROBJECT_HPP

#include "oglframebufferobject.hpp"

namespace mana {
    namespace opengl {
        class OGLUserFrameBufferObject : public OGLFrameBufferObject {
        public:
            GLFWwindow *wndH;

            GLuint FBO;

            int width;
            int height;

            GLuint colorBuffer;
            GLuint renderBuffer;

            bool userTexture = false;

            OGLUserFrameBufferObject() : FBO(0),
                                         width(0),
                                         height(0),
                                         wndH(nullptr),
                                         colorBuffer(0),
                                         renderBuffer(0),
                                         userTexture(false) {}

            OGLUserFrameBufferObject(GLFWwindow *wndH, int width, int height)
                    : wndH(wndH), width(width), height(height), FBO(0), colorBuffer(0), renderBuffer(0),
                      userTexture(false) {}

            ~OGLUserFrameBufferObject() override {
                GLFWwindow *originalContext = glfwGetCurrentContext();
                glfwMakeContextCurrent(wndH);
                glDeleteRenderbuffers(1, &renderBuffer);
                if (!userTexture) {
                    glDeleteTextures(1, &colorBuffer);
                }
                glDeleteFramebuffers(1, &FBO);
                if (originalContext != NULL)
                    glfwMakeContextCurrent(originalContext);
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

#endif //MANA_OGLUSERFRAMEBUFFEROBJECT_HPP
