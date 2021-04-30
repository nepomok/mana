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

#include "engine/ecs/systems/scriptingsystem.hpp"

#include "engine/ecs/components.hpp"

namespace mana {
    void ScriptingSystem::start() {

    }

    void ScriptingSystem::stop() {

    }

    void ScriptingSystem::update(float deltaTime, Scene &scene) {
        auto nodes = scene.findNodesWithComponent<ScriptComponent>();
        for (auto *node : nodes) {
            if (!node->enabled)
                continue;
            auto &comp = node->getComponent<ScriptComponent>();
            comp.script->onUpdate();
        }
    }
}