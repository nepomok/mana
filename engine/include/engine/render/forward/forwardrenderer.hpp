#ifndef MANA_FORWARDRENDERER_HPP
#define MANA_FORWARDRENDERER_HPP

#include "engine/asset/scene.hpp"

namespace engine {
    class ForwardRenderer {
    public:
        void draw(const Scene &scene);
    };
}

#endif //MANA_FORWARDRENDERER_HPP
