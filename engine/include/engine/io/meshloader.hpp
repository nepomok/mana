#ifndef MANA_MESHLOADER_HPP
#define MANA_MESHLOADER_HPP

#include <string>

#include "engine/render/mesh.hpp"

namespace mana {
    class MeshLoader {
    public:
        static Mesh load(std::string filepath);

        static std::vector<Mesh> loadMultiple(std::string filepath);
    };
}

#endif //MANA_MESHLOADER_HPP
