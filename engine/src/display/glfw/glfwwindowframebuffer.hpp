#ifndef MANA_GLFWWINDOWFRAMEBUFFER_HPP
#define MANA_GLFWWINDOWFRAMEBUFFER_HPP

#include "render/opengl/oglframebufferobject.hpp"

namespace mana {
    class GLFWWindowFrameBuffer : public opengl::OGLFrameBufferObject {
    public:
        GLFWwindow *wndH;

        explicit GLFWWindowFrameBuffer(GLFWwindow *wndH) : wndH(wndH) {}

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

#endif //MANA_GLFWWINDOWFRAMEBUFFER_HPP
