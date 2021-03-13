#ifndef MANA_MESHDATA_HPP
#define MANA_MESHDATA_HPP

#include "engine/ecs/component.hpp"
#include "engine/io/resource.hpp"
#include "engine/io/imagebuffer.hpp"
#include "engine/render/rendermesh.hpp"
#include "engine/ecs/data/material.hpp"

namespace mana {

    struct MeshRenderer : public Component {
        Resource<RenderMesh> *mesh = nullptr;
        Material material;
    };
}

#endif //MANA_MESHDATA_HPP
