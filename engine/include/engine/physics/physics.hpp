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

#ifndef MANA_PHYSICS_HPP
#define MANA_PHYSICS_HPP

#include "engine/physics/physicsbackend.hpp"
#include "engine/physics/2d/world2d.hpp"
#include "engine/physics/3d/world3d.hpp"

namespace mana {
    namespace Physics {
        World2D *createWorld2D(PhysicsBackend backend);

        World3D *createWorld3D(PhysicsBackend backend);
    }
}

#endif //MANA_PHYSICS_HPP
