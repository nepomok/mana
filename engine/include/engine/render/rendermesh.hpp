#ifndef MANA_RENDERMESH_HPP
#define MANA_RENDERMESH_HPP

#include "engine/render/renderobject.hpp"

#include "engine/math/transform.hpp"

namespace mana {
    class RenderMesh : public RenderObject {
    public:
        ~RenderMesh() override = default;

        virtual void setOffsets(const std::vector<Transform> &offsets) = 0;
    };
}

#endif //MANA_RENDERMESH_HPP
