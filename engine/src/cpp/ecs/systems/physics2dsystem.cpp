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

#include "engine/ecs/systems/physics2dsystem.hpp"

#include "engine/ecs/components/physics2dcomponent.hpp"

#include "engine/physics/2d/world2d.hpp"

namespace mana {
    Physics2DSystem::Physics2DSystem(const World2D &world) : world(&world) {}

    void Physics2DSystem::start() {}

    void Physics2DSystem::stop() {}

    void Physics2DSystem::update(float deltaTime, Scene &scene) {
        world->step(deltaTime);
        for (auto &node : scene.findNodesWithComponent<Physics2DComponent>()) {
            /*auto &transform = node->getComponent<TransformComponent>();
            auto &physicsComponent = node->getComponent<Physics2DComponent>();
            Vec2f pos = physicsComponent.rigidbody.get().getPosition();
            transform.transform.position = {pos.x, pos.y, transform.transform.position.z};
            transform.transform.rotation.z = physicsComponent.rigidbody.get().getRotation();*/
        }
    }
}