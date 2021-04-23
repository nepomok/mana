#ifndef MANA_RENDERCOMMAND_HPP
#define MANA_RENDERCOMMAND_HPP

#include <vector>

#include "engine/render/rendermesh.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/shaderprogram.hpp"

#include "engine/math/transform.hpp"

namespace mana {
    enum DepthTestMode {
        DEPTH_TEST_ALWAYS,
        DEPTH_TEST_NEVER,
        DEPTH_TEST_LESS,
        DEPTH_TEST_EQUAL,
        DEPTH_TEST_LEQUAL,
        DEPTH_TEST_GREATER,
        DEPTH_TEST_NOTEQUAL,
        DEPTH_TEST_GEQUAL
    };

    enum StencilMode {
        STENCIL_NEVER,
        STENCIL_LESS,
        STENCIL_LEQUAL,
        STENCIL_GREATER,
        STENCIL_GEQUAL,
        STENCIL_EQUAL,
        STENCIL_NOTEQUAL,
        STENCIL_ALWAYS
    };

    enum StencilAction {
        STENCIL_KEEP,
        STENCIL_ZERO,
        STENCIL_REPLACE,
        STENCIL_INCR,
        STENCIL_INCR_WRAP,
        STENCIL_DECR,
        STENCIL_DECR_WRAP,
        STENCIL_INVERT
    };

    enum FaceCullingMode {
        CULL_NONE,
        CULL_FRONT,
        CULL_BACK
    };

    enum BlendMode {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA
    };
    
    struct RenderCommand {
        Transform transform;

        std::vector<RenderMesh *> meshObjects;
        std::vector<RenderTexture *> textureObjects;

        ShaderProgram *shader;

        bool enableDepthTest = false;
        bool depthTestWrite = true;
        DepthTestMode depthTestMode = DepthTestMode::DEPTH_TEST_LESS;

        bool enableStencilTest = false;
        uint stencilTestMask = 0xFF;
        StencilMode stencilMode = StencilMode::STENCIL_ALWAYS;
        int stencilReference = 1;
        uint stencilFunctionMask = 0xFF;
        StencilAction stencilFail = StencilAction::STENCIL_KEEP;
        StencilAction stencilDepthFail = StencilAction::STENCIL_KEEP;
        StencilAction stencilPass = StencilAction::STENCIL_KEEP;

        bool enableFaceCulling = false;
        FaceCullingMode faceCullMode = FaceCullingMode::CULL_FRONT;
        bool faceCullClockwiseWinding = false;

        bool enableBlending = false;
        BlendMode blendSourceMode = BlendMode::ONE;
        BlendMode blendDestinationMode = BlendMode::ZERO;
    };
}

#endif //MANA_RENDERCOMMAND_HPP
