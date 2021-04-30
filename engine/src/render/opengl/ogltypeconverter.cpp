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

#include <stdexcept>

#include "render/opengl/ogltypeconverter.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        namespace OGLTypeConverter {
            GLint convertPrimitive(int value) {
                return (GLint) value;
            }

            GLuint convertPrimitive(uint value) {
                return (GLuint) value;
            }

            GLenum convert(FaceCullingMode mode) {
                switch (mode) {
                    case CULL_NONE:
                        return GL_NONE;
                    case CULL_FRONT:
                        return GL_FRONT;
                    case CULL_BACK:
                        return GL_BACK;
                }
                throw std::runtime_error("Unsupported culling mode");
            }

            GLenum convert(const DepthTestMode &mode) {
                switch (mode) {
                    case DEPTH_TEST_ALWAYS:
                        return GL_ALWAYS;
                    case DEPTH_TEST_NEVER:
                        return GL_NEVER;
                    case DEPTH_TEST_LESS:
                        return GL_LESS;
                    case DEPTH_TEST_EQUAL:
                        return GL_EQUAL;
                    case DEPTH_TEST_LEQUAL:
                        return GL_LEQUAL;
                    case DEPTH_TEST_GREATER:
                        return GL_GREATER;
                    case DEPTH_TEST_NOTEQUAL:
                        return GL_NOTEQUAL;
                    case DEPTH_TEST_GEQUAL:
                        return GL_GEQUAL;
                }
                throw std::runtime_error("Unsupported depth testing mode");
            }

            GLenum convert(BlendMode mode) {
                switch (mode) {
                    case ZERO:
                        return GL_ZERO;
                    case ONE:
                        return GL_ONE;
                    case SRC_COLOR:
                        return GL_SRC_COLOR;
                    case ONE_MINUS_SRC_COLOR:
                        return GL_ONE_MINUS_SRC_COLOR;
                    case DST_COLOR:
                        return GL_DST_COLOR;
                    case SRC_ALPHA:
                        return GL_SRC_ALPHA;
                    case ONE_MINUS_SRC_ALPHA:
                        return GL_ONE_MINUS_SRC_ALPHA;
                    case DST_ALPHA:
                        return GL_DST_ALPHA;
                    case ONE_MINUS_DST_ALPHA:
                        return GL_ONE_MINUS_DST_ALPHA;
                    case CONSTANT_COLOR:
                        return GL_CONSTANT_COLOR;
                    case ONE_MINUS_CONSTANT_COLOR:
                        return GL_ONE_MINUS_CONSTANT_COLOR;
                    case CONSTANT_ALPHA:
                        return GL_CONSTANT_ALPHA;
                    case ONE_MINUS_CONSTANT_ALPHA:
                        return GL_ONE_MINUS_CONSTANT_ALPHA;
                }
                throw std::runtime_error("Unsupported blending mode");
            }

            GLenum convert(StencilMode mode) {
                switch (mode) {
                    case STENCIL_NEVER:
                        return GL_NEVER;
                    case STENCIL_LESS:
                        return GL_LESS;
                    case STENCIL_LEQUAL:
                        return GL_LEQUAL;
                    case STENCIL_GREATER:
                        return GL_GREATER;
                    case STENCIL_GEQUAL:
                        return GL_GEQUAL;
                    case STENCIL_EQUAL:
                        return GL_EQUAL;
                    case STENCIL_NOTEQUAL:
                        return GL_NOTEQUAL;
                    case STENCIL_ALWAYS:
                        return GL_ALWAYS;
                }
                throw std::runtime_error("Unsupported stencil mode");
            }

            GLenum convert(StencilAction action) {
                switch (action) {
                    case STENCIL_KEEP:
                        return GL_KEEP;
                    case STENCIL_ZERO:
                        return GL_ZERO;
                    case STENCIL_REPLACE:
                        return GL_REPLACE;
                    case STENCIL_INCR:
                        return GL_INCR;
                    case STENCIL_INCR_WRAP:
                        return GL_INCR_WRAP;
                    case STENCIL_DECR:
                        return GL_DECR;
                    case STENCIL_DECR_WRAP:
                        return GL_DECR_WRAP;
                    case STENCIL_INVERT:
                        return GL_INVERT;
                }
                throw std::runtime_error("Unsupported stencil action");
            }

            GLenum convert(RenderTexture::TextureType type) {
                switch (type) {
                    case RenderTexture::TEXTURE_2D:
                        return GL_TEXTURE_2D;
                    case RenderTexture::TEXTURE_CUBE_MAP:
                        return GL_TEXTURE_CUBE_MAP;
                    default:
                        throw std::runtime_error("Unrecognized texture type");
                }
            }

            GLenum convert(RenderTexture::CubeMapFace face) {
                switch (face) {
                    case RenderTexture::FRONT:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                    case RenderTexture::BACK:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                    case RenderTexture::LEFT:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                    case RenderTexture::RIGHT:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                    case RenderTexture::TOP:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                    case RenderTexture::BOTTOM:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                    default:
                        throw std::runtime_error("Unrecognized cube map face");
                }
            }

            GLenum convert(RenderTexture::ColorFormat format) {
                switch (format) {
                    case RenderTexture::RGB:
                        return GL_RGB;
                    case RenderTexture::RGBA:
                        return GL_RGBA;
                    case RenderTexture::DEPTH:
                        return GL_DEPTH_COMPONENT;
                    case RenderTexture::DEPTH_STENCIL:
                        return GL_DEPTH_STENCIL;
                    case RenderTexture::RGB_COMPRESSED:
                        return GL_COMPRESSED_RGB;
                    case RenderTexture::RGBA_COMPRESSED:
                        return GL_COMPRESSED_RGBA;
                    default:
                        throw std::runtime_error("Unrecognized color format");
                }
            }

            GLint convert(RenderTexture::TextureWrapping wrapping) {
                switch (wrapping) {
                    case RenderTexture::REPEAT:
                        return GL_REPEAT;
                    case RenderTexture::MIRRORED_REPEAT:
                        return GL_MIRRORED_REPEAT;
                    case RenderTexture::CLAMP_TO_EDGE:
                        return GL_CLAMP_TO_EDGE;
                    case RenderTexture::CLAMP_TO_BORDER:
                        return GL_CLAMP_TO_BORDER;
                }
                throw std::runtime_error("Invalid texture wrapping");
            }

            GLint convert(RenderTexture::TextureFiltering filtering) {
                switch (filtering) {
                    case RenderTexture::NEAREST:
                        return GL_NEAREST;
                    case RenderTexture::LINEAR:
                        return GL_LINEAR;
                }
                throw std::runtime_error("Invalid texture filtering");
            }

            GLint convert(RenderTexture::MipMapFiltering filtering) {
                switch (filtering) {
                    case RenderTexture::NEAREST_MIPMAP_NEAREST:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case RenderTexture::LINEAR_MIPMAP_NEAREST:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case RenderTexture::NEAREST_MIPMAP_LINEAR:
                        return GL_NEAREST_MIPMAP_LINEAR;
                    case RenderTexture::LINEAR_MIPMAP_LINEAR:
                        return GL_LINEAR_MIPMAP_LINEAR;
                }
                throw std::runtime_error("Invalid mipmap filtering");
            }
        }
    }
}