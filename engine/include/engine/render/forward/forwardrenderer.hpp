#ifndef MANA_FORWARDRENDERER_HPP
#define MANA_FORWARDRENDERER_HPP

#include "engine/asset/scene.hpp"

#include "engine/render/api/renderer.hpp"

namespace engine {
    namespace ForwardRenderer {
        void renderScene(Renderer &ren, RenderTarget &target, Scene &scene);
    }
}

#endif //MANA_FORWARDRENDERER_HPP
