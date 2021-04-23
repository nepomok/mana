#ifndef MANA_RENDERER_HPP
#define MANA_RENDERER_HPP

#include "engine/common/imagebuffer.hpp"
#include "engine/render/rendercommand.hpp"
#include "engine/render/framebuffer.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/render/rendertexture.hpp"
#include "engine/render/renderscene.hpp"

namespace mana {
    class Renderer {
    public:
        virtual void render(const FrameBuffer &frameBuffer,
                            const RenderScene &scene,
                            Vec2i viewportOffset,
                            Vec2i viewportSize,
                            ColorRGBA32 clearColorValue,
                            bool clearColor,
                            bool clearDepth,
                            bool clearStencil,
                            bool multiSample) = 0;

        virtual void render(const FrameBuffer &frameBuffer,
                            const RenderScene &scene,
                            ColorRGBA32 clearColorValue = ColorRGBA32(0),
                            bool clearColor = true,
                            bool clearDepth = true,
                            bool clearStencil = true,
                            bool multiSample = true) = 0;
    };
}

#endif //MANA_RENDERER_HPP
