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

#include "render/opengl/oglrendertexture.hpp"
#include "render/opengl/oglcheckerror.hpp"
#include "render/opengl/ogltypeconverter.hpp"

using namespace mana;
using namespace mana::opengl;

OGLRenderTexture::OGLRenderTexture(int width, int height) : width(width), height(height), handle(), isCubeMap(false) {}

OGLRenderTexture::~OGLRenderTexture() {
    glDeleteTextures(1, &handle);
}

Vec2i OGLRenderTexture::getSize() {
    return {width, height};
}

void OGLRenderTexture::upload(const ImageBuffer<ColorRGB> &buffer, ColorFormat internalFormat) {
    if (buffer.getWidth() != width || buffer.getHeight() != height) {
        throw std::runtime_error("Attempted to write input buffer with non matching size");
    }

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 OGLTypeConverter::convert(internalFormat),
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer.buffer.getData());
    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLError("OGLRenderTexture::upload(RGB)");
}

void OGLRenderTexture::upload(const ImageBuffer<ColorRGBA> &buffer, ColorFormat internalFormat) {
    if (buffer.getWidth() != width || buffer.getHeight() != height) {
        throw std::runtime_error("Attempted to write input buffer with non matching size");
    }

    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D,
                    0,
                    OGLTypeConverter::convert(internalFormat),
                    width,
                    height,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    buffer.buffer.getData());
    glBindTexture(GL_TEXTURE_2D, 0);

    checkGLError("OGLRenderTexture::upload(RGBA)");
}

mana::ImageBuffer<ColorRGBA> OGLRenderTexture::download() {
    auto output = ImageBuffer<ColorRGBA>(width, height);
    glBindTexture(GL_TEXTURE_2D, handle);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, output.buffer.getData());
    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("OGLRenderTexture::download");
    return output;
}

void OGLRenderTexture::upload(CubeMapFace face, const ImageBuffer<ColorRGBA> &buffer, ColorFormat internalFormat) {
    if (buffer.getWidth() != width || buffer.getHeight() != height) {
        throw std::runtime_error("Attempted to write input buffer with non matching size");
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);
    glTexImage2D(OGLTypeConverter::convert(face),
                 0,
                 OGLTypeConverter::convert(internalFormat),
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 buffer.buffer.getData());
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    checkGLError("OGLRenderTexture::upload(CUBEMAP)");
}

ImageBuffer<ColorRGBA> OGLRenderTexture::download(RenderTexture::CubeMapFace face) {
    throw std::runtime_error("Not Implemented");
}
