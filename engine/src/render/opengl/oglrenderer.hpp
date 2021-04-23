#ifndef MANA_OGLRENDERER_HPP
#define MANA_OGLRENDERER_HPP

#include "engine/render/renderer.hpp"

namespace mana {
    namespace opengl {
        class OGLRenderer : public Renderer {
        public:
            void render(const FrameBuffer &frameBuffer,
                        const RenderScene &scene,
                        Vec2i viewportOffset,
                        Vec2i viewportSize,
                        ColorRGBA clearColorValue,
                        bool clearColor,
                        bool clearDepth,
                        bool clearStencil,
                        bool multiSample) override;

            void render(const FrameBuffer &frameBuffer,
                        const RenderScene &scene,
                        ColorRGBA clearColorValue,
                        bool clearColor,
                        bool clearDepth,
                        bool clearStencil,
                        bool multiSample) override;
        };
    }
}

#endif //MANA_OGLRENDERER_HPP
