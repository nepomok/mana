#ifndef MANA_OGLRENDERTEXTURE_HPP
#define MANA_OGLRENDERTEXTURE_HPP

#include "engine/render/rendertexture.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLRenderTexture : public RenderTexture {
        public:
            int width, height;
            GLuint handle;
            bool isCubeMap;

            OGLRenderTexture(int width, int height);

            OGLRenderTexture(const OGLRenderTexture &copy) = delete;

            OGLRenderTexture &operator=(const OGLRenderTexture &copy) = delete;

            ~OGLRenderTexture() override;

            Vec2i getSize() override;

            void upload(const ImageBuffer <ColorRGB> &buffer, ColorFormat internalFormat) override;

            void upload(const ImageBuffer <ColorRGBA> &buffer, ColorFormat internalFormat) override;

            ImageBuffer<ColorRGBA> download() override;

            void upload(CubeMapFace face, const ImageBuffer <ColorRGBA> &buffer, ColorFormat internalFormat) override;

            ImageBuffer<ColorRGBA> download(CubeMapFace face) override;
        };
    }
}

#endif //MANA_OGLRENDERTEXTURE_HPP
