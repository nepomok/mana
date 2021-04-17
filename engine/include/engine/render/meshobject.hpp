#ifndef MANA_MESHOBJECT_HPP
#define MANA_MESHOBJECT_HPP

#include "engine/render/renderobject.hpp"

namespace mana {
    class MeshObject : public RenderObject {
    public:
        ~MeshObject() override = default;
    };
}

#endif //MANA_MESHOBJECT_HPP
