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
