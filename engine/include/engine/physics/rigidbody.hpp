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

#include "engine/physics/collider.hpp"

namespace mana
{
    template<typename T>
    class RigidBody
    {
    public:
        enum Type {
            STATIC,
            KINEMATIC,
            DYNAMIC
        };

        virtual ~RigidBody() = 0;

        virtual void setRigidBodyType(Type type) const = 0;

        virtual Type getRigidBodyType() const = 0;

        virtual void setPosition(const T &position) const = 0;

        virtual const T &getPosition() const = 0;

        virtual void setVelocity(const T &velocity) const = 0;

        virtual const T &getVelocity() const = 0;

        virtual void setRotation(const T &rotation) const = 0;

        virtual const T &getRotation() const = 0;

        virtual void setAngularVelocity(const T &angularVelocity) const = 0;

        virtual const T &getAngularVelocity() const = 0;

        virtual void setColliders(const std::vector<Collider<T>> &collider) const = 0;
    };

    typedef RigidBody<Vec2f> RigidBody2D;
    typedef RigidBody<Vec3f> RigidBody3D;
}
#endif //MANA_RIGIDBODY_HPP
