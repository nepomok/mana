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

#ifndef MANA_WORLD3D_HPP
#define MANA_WORLD3D_HPP

#include <vector>

#include "engine/physics/3d/rigidbody3d.hpp"
#include "engine/math/vector3.hpp"

namespace mana {
    class World3D {
    public:
        class CollisionListener {
        public:
            virtual ~CollisionListener() = default;

            //TODO: Add Collision events
        };

        virtual ~World3D() = 0;

        virtual RigidBody3D *createRigidBody() = 0;

        virtual Collider3D *createCollider() = 0;

        virtual void addCollisionListener(CollisionListener *listener) = 0;

        virtual void removeCollisionListener(CollisionListener *listener) = 0;

        virtual void setGravity(const Vec3f &gravity) = 0;

        virtual void step(float deltaTime) const = 0;
    };
}

#endif //MANA_WORLD3D_HPP
