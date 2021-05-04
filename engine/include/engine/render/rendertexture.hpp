/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_RENDERTEXTURE_HPP
#define MANA_RENDERTEXTURE_HPP

#include "engine/render/imagebuffer.hpp"
#include "engine/render/renderobject.hpp"

namespace mana {
    class RenderTexture : public RenderObject {
    public:
        enum TextureType {
            TEXTURE_2D,
            TEXTURE_CUBE_MAP
        };

        enum CubeMapFace {
            RIGHT,
            LEFT,
            TOP,
            BOTTOM,
            FRONT,
            BACK
        };

        enum ColorFormat {
            RGB,
            RGBA,

            DEPTH,
            DEPTH_STENCIL,

            RGB_COMPRESSED,
            RGBA_COMPRESSED
        };

        enum TextureWrapping {
            REPEAT,
            MIRRORED_REPEAT,
            CLAMP_TO_EDGE,
            CLAMP_TO_BORDER
        };

        enum TextureFiltering {
            NEAREST,
            LINEAR
        };

        enum MipMapFiltering {
            NEAREST_MIPMAP_NEAREST,
            LINEAR_MIPMAP_NEAREST,
            NEAREST_MIPMAP_LINEAR,
            LINEAR_MIPMAP_LINEAR
        };

        struct Attributes {
            TextureType textureType = TEXTURE_2D;
            Vec2i size = {};
            ColorFormat format = RGBA;
            TextureWrapping wrapping = REPEAT;
            TextureFiltering filterMin = LINEAR;
            TextureFiltering filterMag = LINEAR;
            bool generateMipmap = false;
            MipMapFiltering mipmapFilter = NEAREST_MIPMAP_NEAREST;
        };

        const Attributes attributes;

        explicit RenderTexture(Attributes attributes) : attributes(attributes) {}

        ~RenderTexture() override = default;

        virtual void upload(const ImageBuffer<ColorRGB> &buffer) = 0;

        virtual void upload(const ImageBuffer<ColorRGBA> &buffer) = 0;

        virtual ImageBuffer<ColorRGBA> download() = 0;

        virtual void upload(CubeMapFace face,
                            const ImageBuffer<ColorRGBA> &buffer) = 0;

        virtual ImageBuffer<ColorRGBA> download(CubeMapFace face) = 0;

        virtual void uploadCubeMap(const ImageBuffer<ColorRGBA> &buffer) = 0;

        virtual ImageBuffer<ColorRGBA> downloadCubeMap() = 0;
    };
}

#endif //MANA_RENDERTEXTURE_HPP
