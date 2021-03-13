#ifndef MANA_OGLTEXTUREOBJECT_HPP
#define MANA_OGLTEXTUREOBJECT_HPP

#include "engine/render/textureobject.hpp"

#include "../../../extern/glad.h"
#include <GLFW/glfw3.h>

namespace mana {
    namespace opengl {
        class OGLTextureObject : public TextureObject {
        public:
            int width, height;
            GLuint handle;
            GLFWwindow *wndH;
            bool cubeMap = false;

            OGLTextureObject(GLFWwindow *wndH, int width, int height) : handle(0), wndH(wndH), width(width),
                                                                        height(height) {}

            OGLTextureObject(const OGLTextureObject &copy) = delete;

            OGLTextureObject &operator=(const OGLTextureObject &copy) = delete;

            ~OGLTextureObject() override {
                GLFWwindow *originalContext = glfwGetCurrentContext();
                glfwMakeContextCurrent(wndH);
                glDeleteTextures(1, &handle);
                if (originalContext != NULL)
                    glfwMakeContextCurrent(originalContext);
            }
        };
    }
}

#endif //MANA_OGLTEXTUREOBJECT_HPP
