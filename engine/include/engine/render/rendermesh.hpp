#ifndef MANA_RENDERMESH_HPP
#define MANA_RENDERMESH_HPP

#include "engine/render/renderobject.hpp"

#include "engine/math/transform.hpp"

namespace mana {
    class RenderMesh : public RenderObject {
    public:
        ~RenderMesh() override = default;
    };
}

#endif //MANA_RENDERMESH_HPP
