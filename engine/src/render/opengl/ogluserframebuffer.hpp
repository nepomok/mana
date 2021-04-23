#ifndef MANA_OGLUSERFRAMEBUFFER_HPP
#define MANA_OGLUSERFRAMEBUFFER_HPP

#include "render/opengl/oglframebuffer.hpp"

namespace mana {
    namespace opengl {
        class OGLUserFrameBuffer : public OGLFrameBuffer {
        public:
            GLuint FBO;

            int width;
            int height;

            GLuint colorBuffer;
            GLuint renderBuffer;

            bool userTexture = false;

            OGLUserFrameBuffer();

            OGLUserFrameBuffer(int width, int height);

            ~OGLUserFrameBuffer() override;

            Vec2i getSize() const override;

            void blitColor(const FrameBuffer &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureFiltering filter) override;

            void blitDepth(const FrameBuffer &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureFiltering filter) override;

            void blitStencil(const FrameBuffer &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             TextureFiltering filter) override;

            void attachColor(const RenderTexture &texture) override;

            void attachDepth(const RenderTexture &texture) override;

            void attachStencil(const RenderTexture &texture) override;

            void attachDepthStencil(const RenderTexture &texture) override;

            void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) override;

            void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) override;

            void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override;

            void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override;

            GLuint getFBO() const override;
        };
    }
}

#endif //MANA_OGLUSERFRAMEBUFFER_HPP
