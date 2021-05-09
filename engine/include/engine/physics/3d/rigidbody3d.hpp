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

#ifndef MANA_RIGIDBODY_HPP
#define MANA_RIGIDBODY_HPP

#include "engine/physics/3d/collider3d.hpp"
#include "engine/physics/rigidbodytype.hpp"
#include "engine/math/vector3.hpp"

namespace mana {
    class World3D;

    class Rigidbody3D {
    public:
        virtual ~Rigidbody3D() = 0;

        virtual World3D *getWorld() = 0;

        virtual void setRigidbodyType(RigidBodyType type) = 0;

        virtual RigidBodyType getRigidbodyType() = 0;

        virtual void setPosition(const Vec3f &position) = 0;

        virtual Vec3f getPosition() = 0;

        virtual void setVelocity(const Vec3f &velocity) = 0;

        virtual Vec3f getVelocity() = 0;

        virtual void setAngularVelocity(const Vec3f &angularVelocity) = 0;

        virtual Vec3f getAngularVelocity() = 0;

        virtual void setColliders(const std::vector<Collider3D> &collider) = 0;
    };
}

#endif //MANA_RIGIDBODY_HPP
