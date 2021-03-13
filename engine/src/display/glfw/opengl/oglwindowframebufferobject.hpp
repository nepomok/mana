#ifndef MANA_OGLWINDOWFRAMEBUFFEROBJECT_HPP
#define MANA_OGLWINDOWFRAMEBUFFEROBJECT_HPP

#include "oglframebufferobject.hpp"

#include "../glfwinclude.hpp"

namespace mana {
    namespace opengl {
        class OGLWindowFrameBufferObject : public OGLFrameBufferObject {
        public:
            GLFWwindow *wndH;

            explicit OGLWindowFrameBufferObject(GLFWwindow *wndH) : wndH(wndH) {}

            Vec2i getSize() const override {
                Vec2i ret;
                glfwGetWindowSize(wndH, &ret.x, &ret.y);
                return ret;
            }

            GLuint getFBO() const override {
                return 0;
            }
        };
    }
}

#endif //MANA_OGLWINDOWFRAMEBUFFEROBJECT_HPP
