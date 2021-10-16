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

#ifndef MANA_QTOGLTEXTUREBUFFER_HPP
#define MANA_QTOGLTEXTUREBUFFER_HPP

#include "engine/render/texturebuffer.hpp"

#include "openglinclude.hpp"

#include <QOpenGLFunctions_3_3_Core>

namespace engine {
    namespace opengl {
        class QtOGLTextureBuffer : public TextureBuffer, public QOpenGLFunctions_3_3_Core {
        public:
            GLuint handle;

            explicit QtOGLTextureBuffer(Attributes attributes);

            QtOGLTextureBuffer(const QtOGLTextureBuffer &copy) = delete;

            QtOGLTextureBuffer &operator=(const QtOGLTextureBuffer &copy) = delete;

            ~QtOGLTextureBuffer() override;

            TextureBuffer * upload(const Image <ColorRGB> &buffer) override;

            TextureBuffer * upload(const Image <ColorRGBA> &buffer) override;

            TextureBuffer * upload(const Image<float> &buffer) override;

            TextureBuffer * upload(const Image<int> &buffer) override;

            TextureBuffer * upload(const Image<char> &buffer) override;

            TextureBuffer * upload(const Image<unsigned char> &buffer) override;

            Image<ColorRGBA> download() override;

            TextureBuffer * upload(CubeMapFace face, const Image <ColorRGBA> &buffer) override;

            Image<ColorRGBA> download(CubeMapFace face) override;

            TextureBuffer * uploadCubeMap(const Image<ColorRGBA> &buffer) override;

            Image<ColorRGBA> downloadCubeMap() override;

        private:
            void setTextureType(TextureType type);
        };
    }
}

#endif //MANA_QTOGLTEXTUREBUFFER_HPP