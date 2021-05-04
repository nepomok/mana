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

#ifndef MANA_OGLRENDERTEXTURE_HPP
#define MANA_OGLRENDERTEXTURE_HPP

#include "engine/render/texture.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        class OGLRenderTexture : public Texture {
        public:
            GLuint handle;

            explicit OGLRenderTexture(Attributes attributes);

            OGLRenderTexture(const OGLRenderTexture &copy) = delete;

            OGLRenderTexture &operator=(const OGLRenderTexture &copy) = delete;

            ~OGLRenderTexture() override;

            void upload(const ImageBuffer <ColorRGB> &buffer) override;

            void upload(const ImageBuffer <ColorRGBA> &buffer) override;

            ImageBuffer<ColorRGBA> download() override;

            void upload(CubeMapFace face, const ImageBuffer <ColorRGBA> &buffer) override;

            ImageBuffer<ColorRGBA> download(CubeMapFace face) override;

            void uploadCubeMap(const ImageBuffer<ColorRGBA> &buffer) override;

            ImageBuffer<ColorRGBA> downloadCubeMap() override;
        };
    }
}

#endif //MANA_OGLRENDERTEXTURE_HPP
