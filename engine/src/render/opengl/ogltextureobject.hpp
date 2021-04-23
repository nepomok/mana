#ifndef MANA_OGLTEXTUREOBJECT_HPP
#define MANA_OGLTEXTUREOBJECT_HPP

#include "engine/render/rendertexture.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLTextureObject : public RenderTexture {
        public:
            int width, height;
            GLuint handle;
            bool cubeMap = false;

            OGLTextureObject(int width, int height) : handle(0),
                                                      width(width),
                                                      height(height) {}

            OGLTextureObject(const OGLTextureObject &copy) = delete;

            OGLTextureObject &operator=(const OGLTextureObject &copy) = delete;

            ~OGLTextureObject() override {
                glDeleteTextures(1, &handle);
            }
        };
    }
}

#endif //MANA_OGLTEXTUREOBJECT_HPP
