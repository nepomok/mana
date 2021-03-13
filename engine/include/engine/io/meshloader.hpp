#ifndef MANA_MESHLOADER_HPP
#define MANA_MESHLOADER_HPP

#include <string>

#include "engine/render/rendermesh.hpp"

namespace mana {
    class MeshLoader {
    public:
        static RenderMesh load(std::string filepath);

        static std::vector<RenderMesh> loadMultiple(std::string filepath);
    };
}

#endif //MANA_MESHLOADER_HPP
