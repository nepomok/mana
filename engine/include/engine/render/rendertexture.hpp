#ifndef MANA_RENDERTEXTURE_HPP
#define MANA_RENDERTEXTURE_HPP

#include "engine/render/imagebuffer.hpp"
#include "engine/render/renderobject.hpp"
#include "engine/render/textureattributes.hpp"

namespace mana {
    class RenderTexture : public RenderObject {
    public:
        enum CubeMapFace {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            TOP,
            BOTTOM
        };

        ~RenderTexture() override = default;

        virtual Vec2i getSize() = 0;

        virtual void upload(const ImageBuffer<ColorRGB> &buffer, ColorFormat internalFormat = RGBA) = 0;

        virtual void upload(const ImageBuffer<ColorRGBA> &buffer, ColorFormat internalFormat = RGBA) = 0;

        virtual ImageBuffer<ColorRGBA> download() = 0;

        virtual void upload(CubeMapFace face,
                            const ImageBuffer<ColorRGBA> &buffer,
                            ColorFormat internalFormat = RGBA) = 0;

        virtual ImageBuffer<ColorRGBA> download(CubeMapFace face) = 0;
    };
}

#endif //MANA_RENDERTEXTURE_HPP
