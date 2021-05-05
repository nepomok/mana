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

#ifndef MANA_COMPONENTRESOURCE_HPP
#define MANA_COMPONENTRESOURCE_HPP

#include "engine/resource/resource.hpp"
#include "engine/ecs/component.hpp"

namespace mana {
    class ComponentResource : public Resource {
    public:
        /**
         * Allocates and returns a new Component object.
         *
         * The callee gains ownership of the returned pointer.
         *
         * @return
         */
        virtual Component *getComponent() = 0;
    };
}

#endif //MANA_COMPONENTRESOURCE_HPP
