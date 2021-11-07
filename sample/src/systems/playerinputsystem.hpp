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

#ifndef MANA_PLAYERINPUTSYSTEM_HPP
#define MANA_PLAYERINPUTSYSTEM_HPP

#include "engine/ecs/system.hpp"

#include "components/playercontrollercomponent.hpp"

using namespace engine;

class PlayerInputSystem : public System {
public:
    explicit PlayerInputSystem(Input &input) : input(input) {};

    ~PlayerInputSystem() override = default;

    void update(float deltaTime, EntityManager &entityManager) override {
        auto movement = getMovementInput();
        auto rotation = getRotationInput();

        auto &componentManager = entityManager.getComponentManager();
        for (auto &pair: componentManager.getPool<PlayerControllerComponent>()) {
            auto transform = componentManager.lookup<TransformComponent>(pair.first);

            // Unit vectors point to the opposite because
            // the camera is facing in the negative z although positive z is "forward" in world space.
            Vec3f forward = transform.transform.rotate(Vec3f(0, 0, -1));
            Vec3f left = transform.transform.rotate(Vec3f(-1, 0, 0));
            Vec3f up = transform.transform.rotate(Vec3f(0, 1, 0));

            Vec3f relativeMovement = forward * movement.z + left * movement.x + up * movement.y;

            Vec3f worldRot(0, rotation.y, 0);
            Vec3f localRot(rotation.x, 0, 0);

            transform.transform.position += relativeMovement * pair.second.movementSpeed * deltaTime;
            //Apply the world rotation by converting it to a quaternion and using it as multiplier
            transform.transform.rotation = transform.transform.rotation
                                           * Quaternion(worldRot * pair.second.rotationSpeed * deltaTime);
            //Apply the local rotation by converting it to a quaternion and using the existing rotation as multiplier
            transform.transform.rotation = Quaternion(localRot * pair.second.rotationSpeed * deltaTime)
                                           * transform.transform.rotation;

            componentManager.update<TransformComponent>(pair.first, transform);
        }
    }

private:
    Input &input;

    Vec3f getMovementInput() {
        Vec3f ret;
        if (input.getKey(keyboard::KEY_W))
            ret.z = 1;
        else if (input.getKey(keyboard::KEY_S))
            ret.z = -1;
        if (input.getKey(keyboard::KEY_A))
            ret.x = 1;
        else if (input.getKey(keyboard::KEY_D))
            ret.x = -1;
        if (input.getKey(keyboard::KEY_E))
            ret.y = 1;
        else if (input.getKey(keyboard::KEY_Q))
            ret.y = -1;
        return ret;
    }

    Vec3f getRotationInput() {
        Vec3f ret;
        if (input.getKey(keyboard::KEY_UP))
            ret.x = 1;
        else if (input.getKey(keyboard::KEY_DOWN))
            ret.x = -1;
        if (input.getKey(keyboard::KEY_LEFT))
            ret.y = -1;
        else if (input.getKey(keyboard::KEY_RIGHT))
            ret.y = 1;
        return ret;
    }
};

#endif //MANA_PLAYERINPUTSYSTEM_HPP
