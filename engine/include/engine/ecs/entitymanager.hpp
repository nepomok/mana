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

#ifndef MANA_ENTITYMANAGER_HPP
#define MANA_ENTITYMANAGER_HPP

#include <set>
#include <limits>

#include "engine/ecs/componentmanager.hpp"

namespace engine {
    //TODO: Entity name mapping
    class EntityManager {
    public:
        EntityManager() = default;

        ~EntityManager() = default;

        EntityManager(const EntityManager &other) {
            idStore = other.idStore;
            idCounter = other.idCounter;
            componentManager = other.componentManager;
        }

        Entity create() {
            if (idStore.empty()) {
                if (idCounter == std::numeric_limits<int>::max())
                    throw std::runtime_error("Cannot create entity, id overflow");
                auto ret = Entity(idCounter++);
                entities.insert(ret);
                return ret;
            } else {
                auto it = idStore.begin();
                Entity ret(*it);
                idStore.erase(it);
                entities.insert(ret);
                return ret;
            }
        }

        void destroy(const Entity &entity) {
            componentManager.destroy(entity);
            idStore.insert(entity.id);
            entities.erase(entity);
        }

        void clear() {
            componentManager.clear();
            idStore.clear();
            idCounter = 0;
            entities.clear();
        }

        const std::set<Entity> &getEntities() const {
            return entities;
        }

        ComponentManager &getComponentManager() {
            return componentManager;
        }

        const ComponentManager &getComponentManager() const {
            return componentManager;
        }

    private:
        std::set<int> idStore;
        int idCounter = 0;
        std::set<Entity> entities;
        ComponentManager componentManager;
    };
}

#endif //MANA_ENTITYMANAGER_HPP
