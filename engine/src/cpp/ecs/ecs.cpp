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

#include "engine/ecs/ecs.hpp"

#include <utility>
#include <algorithm>

namespace mana {
    ECS::ECS() {}

    ECS::~ECS() {
        for (auto *system : systems) {
            delete system;
        }
    }

    void ECS::addSystem(System *system) {
        systems.emplace_back(system);
        system->start();
    }

    void ECS::removeSystem(System *system) {
        auto it = std::find(systems.begin(), systems.end(), system);
        if (it == systems.end())
            throw std::runtime_error("System not found");
        system->stop();
        systems.erase(it);
    }

    void ECS::update(float deltaTime, Scene &scene) {
        for (auto *system : systems) {
            system->update(deltaTime, scene);
        }
    }
}