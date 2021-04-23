/**
 *  Mana - 3D Engine
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

        virtual void upload(const ImageBuffer<ColorRGB> &buffer, ColorFormat internalFormat = RGB) = 0;

        virtual void upload(const ImageBuffer<ColorRGBA> &buffer, ColorFormat internalFormat = RGB) = 0;

        virtual ImageBuffer<ColorRGBA> download() = 0;

        virtual void upload(CubeMapFace face,
                            const ImageBuffer<ColorRGBA> &buffer,
                            ColorFormat internalFormat = RGBA) = 0;

        virtual ImageBuffer<ColorRGBA> download(CubeMapFace face) = 0;
    };
}

#endif //MANA_RENDERTEXTURE_HPP
