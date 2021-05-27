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

#include "ogltypeconverter.hpp"

#include "openglinclude.hpp"

namespace mana {
    namespace opengl {
        namespace OGLTypeConverter {
            GLenum getColorAttachment(int index) {
                return GL_COLOR_ATTACHMENT0 + index;
            }

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

            GLenum convert(TextureBuffer::TextureType type) {
                switch (type) {
                    case TextureBuffer::TEXTURE_2D:
                        return GL_TEXTURE_2D;
                    case TextureBuffer::TEXTURE_CUBE_MAP:
                        return GL_TEXTURE_CUBE_MAP;
                    default:
                        throw std::runtime_error("Unrecognized texture type");
                }
            }

            GLenum convert(TextureBuffer::CubeMapFace face) {
                switch (face) {
                    case TextureBuffer::FRONT:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                    case TextureBuffer::BACK:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                    case TextureBuffer::LEFT:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                    case TextureBuffer::RIGHT:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                    case TextureBuffer::TOP:
                        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                    case TextureBuffer::BOTTOM:
                        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                    default:
                        throw std::runtime_error("Unrecognized cube map face");
                }
            }

            GLenum convert(TextureBuffer::ColorFormat format) {
                switch (format) {
                    case TextureBuffer::RGB:
                        return GL_RGB;
                    case TextureBuffer::RGBA:
                        return GL_RGBA;
                    case TextureBuffer::DEPTH:
                        return GL_DEPTH_COMPONENT;
                    case TextureBuffer::DEPTH_STENCIL:
                        return GL_DEPTH_STENCIL;
                    case TextureBuffer::RGB_COMPRESSED:
                        return GL_COMPRESSED_RGB;
                    case TextureBuffer::RGBA_COMPRESSED:
                        return GL_COMPRESSED_RGBA;
                    default:
                        throw std::runtime_error("Unrecognized color format");
                }
            }

            GLint convert(TextureBuffer::TextureWrapping wrapping) {
                switch (wrapping) {
                    case TextureBuffer::REPEAT:
                        return GL_REPEAT;
                    case TextureBuffer::MIRRORED_REPEAT:
                        return GL_MIRRORED_REPEAT;
                    case TextureBuffer::CLAMP_TO_EDGE:
                        return GL_CLAMP_TO_EDGE;
                    case TextureBuffer::CLAMP_TO_BORDER:
                        return GL_CLAMP_TO_BORDER;
                }
                throw std::runtime_error("Invalid texture wrapping");
            }

            GLint convert(TextureBuffer::TextureFiltering filtering) {
                switch (filtering) {
                    case TextureBuffer::NEAREST:
                        return GL_NEAREST;
                    case TextureBuffer::LINEAR:
                        return GL_LINEAR;
                }
                throw std::runtime_error("Invalid texture filtering");
            }

            GLint convert(TextureBuffer::MipMapFiltering filtering) {
                switch (filtering) {
                    case TextureBuffer::NEAREST_MIPMAP_NEAREST:
                        return GL_NEAREST_MIPMAP_NEAREST;
                    case TextureBuffer::LINEAR_MIPMAP_NEAREST:
                        return GL_LINEAR_MIPMAP_NEAREST;
                    case TextureBuffer::NEAREST_MIPMAP_LINEAR:
                        return GL_NEAREST_MIPMAP_LINEAR;
                    case TextureBuffer::LINEAR_MIPMAP_LINEAR:
                        return GL_LINEAR_MIPMAP_LINEAR;
                }
                throw std::runtime_error("Invalid mipmap filtering");
            }
        }
    }
}