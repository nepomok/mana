#ifndef MANA_RENDERTEXTURE_HPP
#define MANA_RENDERTEXTURE_HPP

#include "engine/render/imagebuffer.hpp"
#include "engine/render/renderobject.hpp"
#include "engine/render/textureattributes.hpp"

namespace mana {
    class RenderTexture : public RenderObject {
    public:
        ~RenderTexture() override = default;

        virtual Vec2i getSize() = 0;

        virtual void upload(ColorFormat internalFormat, const ImageBuffer<ColorRGB> &buffer) = 0;

        virtual void upload(ColorFormat internalFormat, const ImageBuffer<ColorRGBA> &buffer) = 0;

        virtual ImageBuffer<ColorRGBA> download() = 0;
    };
}

#endif //MANA_RENDERTEXTURE_HPP
