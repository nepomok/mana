/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_SCENE_HPP
#define MANA_SCENE_HPP

#include <vector>
#include <set>

#include "node.hpp"

namespace engine::runtime {
    class Scene {
    public:
        std::string name;
        std::string resources;

        std::map<std::string, Node> nodes;

        Node &operator[](const std::string &nodeName) {
            return nodes[nodeName];
        }

        template<typename T>
        std::vector<Node *> findNodesWithComponent() {
            const std::type_info &typeInfo = typeid(T);
            std::vector<Node *> ret;
            for (auto &node : nodes) {
                if (node.second.hasComponent<T>()) {
                    ret.push_back(&node.second);
                }
            }
            return ret;
        }
    };
}
#endif //MANA_SCENE_HPP
