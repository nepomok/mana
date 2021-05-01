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

#ifndef MANA_SYSTEM_HPP
#define MANA_SYSTEM_HPP

#include "engine/ecs/scene.hpp"

namespace mana {
    /**
     * Systems provide logic.
     *
     * Only systems and scripts invoked by a script system invoke logic on the scene data.
     *
     * Examples are invoking script lifecycle, handle rendering, manage memory allocation etc.
     */
    class System {
    public:
        virtual ~System() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        virtual void update(float deltaTime, Scene &scene) = 0;
    };
}
#endif //MANA_SYSTEM_HPP
