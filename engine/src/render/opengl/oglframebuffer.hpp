#ifndef MANA_OGLFRAMEBUFFER_HPP
#define MANA_OGLFRAMEBUFFER_HPP

#include "engine/render/framebuffer.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLFrameBuffer : public FrameBuffer {
        public:
            ~OGLFrameBuffer() override = default;

            Vec2i getSize() const override = 0;

            void blitColor(const FrameBuffer &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureFiltering filter) override = 0;

            void blitDepth(const FrameBuffer &source,
                           Vec2i sourceOffset,
                           Vec2i targetOffset,
                           Vec2i sourceRect,
                           Vec2i targetRect,
                           TextureFiltering filter) override = 0;

            void blitStencil(const FrameBuffer &source,
                             Vec2i sourceOffset,
                             Vec2i targetOffset,
                             Vec2i sourceRect,
                             Vec2i targetRect,
                             TextureFiltering filter) override = 0;

            void attachColor(const RenderTexture &texture) override = 0;

            void attachDepth(const RenderTexture &texture) override = 0;

            void attachStencil(const RenderTexture &texture) override = 0;

            void attachDepthStencil(const RenderTexture &texture) override = 0;

            void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) override = 0;

            virtual GLuint getFBO() const = 0;
        };
    }
}

#endif //MANA_OGLFRAMEBUFFER_HPP
