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

#include "ogltexturebuffer.hpp"
#include "oglcheckerror.hpp"
#include "ogltypeconverter.hpp"

using namespace mana;
using namespace mana::opengl;

OGLTextureBuffer::OGLTextureBuffer(Attributes attributes) : TextureBuffer(attributes), handle() {
    GLenum type = OGLTypeConverter::convert(attributes.textureType);

    glGenTextures(1, &handle);
    glBindTexture(type, handle);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, OGLTypeConverter::convert(attributes.wrapping));
    glTexParameteri(type, GL_TEXTURE_WRAP_T, OGLTypeConverter::convert(attributes.wrapping));
    checkGLError("OGLTextureBuffer::OGLTextureBuffer()");

    glTexParameteri(type,
                    GL_TEXTURE_MIN_FILTER,
                    OGLTypeConverter::convert(attributes.filterMin));
    glTexParameteri(type,
                    GL_TEXTURE_MAG_FILTER,
                    OGLTypeConverter::convert(attributes.filterMag));
    checkGLError("OGLTextureBuffer::OGLTextureBuffer()");

    if (attributes.textureType == TEXTURE_2D) {
        glTexImage2D(type,
                     0,
                     OGLTypeConverter::convert(attributes.format),
                     attributes.size.x,
                     attributes.size.y,
                     0,
                     OGLTypeConverter::convert(attributes.format),
                     attributes.format == ColorFormat::DEPTH ? GL_FLOAT : GL_UNSIGNED_BYTE,
                     NULL);
    } else {
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         OGLTypeConverter::convert(attributes.format),
                         attributes.size.x,
                         attributes.size.y,
                         0,
                         GL_RGBA,
                         GL_UNSIGNED_BYTE,
                         NULL);
        }
    }
    checkGLError("OGLTextureBuffer::OGLTextureBuffer()");

    if (attributes.generateMipmap) {
        glGenerateMipmap(type);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER,
                        OGLTypeConverter::convert(attributes.mipmapFilter));
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER,
                        OGLTypeConverter::convert(attributes.filterMag));
    }

    glBindTexture(type, 0);

    checkGLError("OGLTextureBuffer::OGLTextureBuffer()");
}

OGLTextureBuffer::~OGLTextureBuffer() {
    glDeleteTextures(1, &handle);
}

void OGLTextureBuffer::upload(const ImageBuffer<ColorRGB> &buffer) {
    if (attributes.textureType != TEXTURE_2D)
        throw std::runtime_error("Invalid texture type");
    if (!(buffer.getSize() == attributes.size))
        throw std::runtime_error("Upload size mismatch");

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 OGLTypeConverter::convert(attributes.format),
                 attributes.size.x,
                 attributes.size.y,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 buffer.getData());

    if (attributes.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLError("OGLTextureBuffer::upload(RGB)");
}

void OGLTextureBuffer::upload(const ImageBuffer<ColorRGBA> &buffer) {
    if (attributes.textureType != TEXTURE_2D)
        throw std::runtime_error("TextureBuffer not texture 2d");
    if (!(buffer.getSize() == attributes.size))
        throw std::runtime_error("Upload size mismatch");

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 OGLTypeConverter::convert(attributes.format),
                 attributes.size.x,
                 attributes.size.y,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer.getData());

    if (attributes.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLError("OGLTextureBuffer::upload(RGBA)");
}

mana::ImageBuffer<ColorRGBA> OGLTextureBuffer::download() {
    if (attributes.textureType != TEXTURE_2D)
        throw std::runtime_error("TextureBuffer not texture 2d");

    auto output = ImageBuffer<ColorRGBA>(attributes.size);
    glBindTexture(GL_TEXTURE_2D, handle);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void *) output.getData());
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("OGLTextureBuffer::download");
    return output;
}

void OGLTextureBuffer::upload(CubeMapFace face, const ImageBuffer<ColorRGBA> &buffer) {
    if (attributes.textureType != TEXTURE_CUBE_MAP)
        throw std::runtime_error("TextureBuffer not cubemap");
    if (!(buffer.getSize() == attributes.size))
        throw std::runtime_error("Upload size mismatch");

    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glTexImage2D(OGLTypeConverter::convert(face),
                 0,
                 OGLTypeConverter::convert(attributes.format),
                 attributes.size.x,
                 attributes.size.y,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer.getData());

    if (attributes.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    checkGLError("OGLTextureBuffer::upload(CUBEMAP)");
}

ImageBuffer<ColorRGBA> OGLTextureBuffer::download(TextureBuffer::CubeMapFace face) {
    if (attributes.textureType != TEXTURE_CUBE_MAP)
        throw std::runtime_error("TextureBuffer not cubemap");

    throw std::runtime_error("Not Implemented");
}

void OGLTextureBuffer::uploadCubeMap(const ImageBuffer<ColorRGBA> &buffer) {
    auto faceSize = buffer.getSize();
    faceSize.x = faceSize.x / 6;
    if (faceSize.x != faceSize.y)
        throw std::runtime_error("Invalid cubemap image");
    if (!(faceSize == attributes.size))
        throw std::runtime_error("Invalid cubemap face size");
    for (int i = 0; i < 6; i++) {
        upload(static_cast<CubeMapFace>(i), buffer.slice(Recti(Vec2i(faceSize.x * i, 0), faceSize)));
    }
}

ImageBuffer<ColorRGBA> OGLTextureBuffer::downloadCubeMap() {
    auto size = attributes.size;
    size.x = size.x * 6;
    ImageBuffer<ColorRGBA> ret(size);
    for (int i = 0; i < 6; i++) {
        ret.blit({Vec2i(i * attributes.size.x, 0), attributes.size}, download(static_cast<CubeMapFace>(i)));
    }
    return ret;
}

