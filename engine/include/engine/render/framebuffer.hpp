#ifndef MANA_FRAMEBUFFER_HPP
#define MANA_FRAMEBUFFER_HPP

#include "engine/render/renderobject.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/textureattributes.hpp"

namespace mana {
    class FrameBuffer : public RenderObject {
    public:
        ~FrameBuffer() override = default;

        virtual Vec2i getSize() const = 0;

        virtual void blitColor(const FrameBuffer &source,
                          Vec2i sourceOffset,
                          Vec2i targetOffset,
                          Vec2i sourceRect,
                          Vec2i targetRect,
                          TextureFiltering filter) = 0;

        virtual void blitDepth(const FrameBuffer &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureFiltering filter) = 0;

        virtual void blitStencil(const FrameBuffer &source,
                               Vec2i sourceOffset,
                               Vec2i targetOffset,
                               Vec2i sourceRect,
                               Vec2i targetRect,
                               TextureFiltering filter) = 0;

        virtual void attachColor(const RenderTexture &texture) = 0;

        virtual void attachDepth(const RenderTexture &texture) = 0;

        virtual void attachStencil(const RenderTexture &texture) = 0;

        virtual void attachDepthStencil(const RenderTexture &texture) = 0;

        virtual void attachColor(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachDepth(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;

        virtual void attachDepthStencil(RenderTexture::CubeMapFace face, const RenderTexture &texture) = 0;
    };
}

#endif //MANA_FRAMEBUFFER_HPP
