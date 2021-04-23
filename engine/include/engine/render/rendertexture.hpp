#ifndef MANA_RENDERTEXTURE_HPP
#define MANA_RENDERTEXTURE_HPP

#include "engine/common/imagebuffer.hpp"
#include "engine/render/renderobject.hpp"

namespace mana {
    class RenderTexture : public RenderObject {
    public:
        ~RenderTexture() override = default;

        virtual Vec2i getSize() = 0;

        virtual void upload(const ImageBuffer <ColorRGBA32> &buffer) = 0;

        virtual ImageBuffer <ColorRGBA32> download() = 0;
    };
}

#endif //MANA_RENDERTEXTURE_HPP
