#ifndef MANA_SCENE_HPP
#define MANA_SCENE_HPP

#include <vector>
#include <set>

#include "engine/ecs/node.hpp"

namespace mana {
    /**
     * A renderCommand object in the form of a vector of nodes.
     *
     * Node hierarchy only exists at component level with transform parenting.
     */
    class Scene {
    public:
        std::vector<Node> nodes;

        template<typename T>
        std::vector<Node> findNodesWithComponent() {
            const std::type_info &typeInfo = typeid(T);
            std::vector<Node> ret;
            for (auto &node : nodes) {
                if (node.hasComponent<T>()) {
                    ret.push_back(node);
                }
            }
            return ret;
        }
    };
}
#endif //MANA_SCENE_HPP
