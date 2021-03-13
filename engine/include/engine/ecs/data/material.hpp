#ifndef MANA_MATERIAL_HPP
#define MANA_MATERIAL_HPP

#include <string>
#include <vector>

#include "engine/ecs/component.hpp"
#include "engine/io/resource.hpp"
#include "engine/io/imagebuffer.hpp"
#include "engine/ecs/data/shader.hpp"

namespace mana {
    struct Material {
        Resource <Shader> *shader = nullptr;
        std::vector<Resource < ImageBuffer < ColorRGBA32>> *> textures;
    };
}
#endif //MANA_MATERIAL_HPP
