#ifndef MANA_RENDERMESHINSTANCED_HPP
#define MANA_RENDERMESHINSTANCED_HPP

#include "engine/render/rendermesh.hpp"

namespace mana {
    class RenderInstancedMesh : public RenderMesh {
    public:
        ~RenderInstancedMesh() override = default;

        virtual void setOffsets(const std::vector<Transform> &offsets) = 0;
    };
}
#endif //MANA_RENDERMESHINSTANCED_HPP
