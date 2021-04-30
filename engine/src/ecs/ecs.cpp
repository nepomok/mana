/**
 *  Mana - 3D Engine
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

#include "engine/ecs/ecs.hpp"

#include <utility>

namespace mana {
    ECS::ECS() : scene() {}

    ECS::ECS(Scene scene) : scene(std::move(scene)) {}

    ECS::~ECS() {
        for (auto *system : systems) {
            delete system;
        }
    }

    void ECS::setScene(Scene s) {
        for (auto *system : systems) {
            for (auto &nodePair : scene.nodes) {
                system->onNodeRemoved(nodePair.second);
            }
        }
        this->scene = std::move(s);
    }

    Scene &ECS::getScene() {
        return scene;
    }

    void ECS::addSystem(System *system) {
        systems.insert(system);
        system->start();
    }

    void ECS::removeSystem(System *system) {
        system->stop();
        systems.erase(system);
    }

    void ECS::update(float deltaTime) {
        for (auto *system : systems) {
            system->update(deltaTime, scene);
        }
    }
}