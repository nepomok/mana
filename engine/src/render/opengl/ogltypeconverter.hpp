#ifndef MANA_OGLTYPECONVERTER_HPP
#define MANA_OGLTYPECONVERTER_HPP

#include "engine/render/textureattributes.hpp"
#include "engine/render/rendercommand.hpp"

#include "extern/glad.h"

namespace mana {
    namespace opengl {
        namespace OGLTypeConverter {
            GLint convertPrimitive(int value);

            GLuint convertPrimitive(uint value);

            GLint convert(TextureWrapping wrapping);

            GLint convert(TextureFiltering filtering);

            GLint convert(MipMapFiltering filtering);

            GLenum convert(FaceCullingMode mode);

            GLenum convert(const DepthTestMode &mode);

            GLenum convert(BlendMode mode);

            GLenum convert(StencilMode mode);

            GLenum convert(StencilAction action);

            GLenum convert(RenderTexture::CubeMapFace face);

            GLenum convert(ColorFormat format);
        }
    }
}

#endif //MANA_OGLTYPECONVERTER_HPP
