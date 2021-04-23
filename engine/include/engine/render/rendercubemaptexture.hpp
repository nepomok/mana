#ifndef MANA_RENDERCUBEMAP_HPP
#define MANA_RENDERCUBEMAP_HPP

#include "engine/render/rendertexture.hpp"

namespace mana {
    class RenderCubeMapTexture : public RenderTexture {
    public:
        enum Face {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            TOP,
            BOTTOM
        };

        ~RenderCubeMapTexture() override = default;

        Vec2i getSize() override = 0;

        void upload(const ImageBuffer <ColorRGBA32> &buffer) override = 0;

        ImageBuffer <ColorRGBA32> download() override = 0;

        virtual void upload(Face face, const ImageBuffer <ColorRGBA32> &buffer) = 0;

        virtual ImageBuffer <ColorRGBA32> download(Face face) = 0;
    };
}

#endif //MANA_RENDERCUBEMAP_HPP
