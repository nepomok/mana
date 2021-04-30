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

#ifndef MANA_RESOURCE_HPP
#define MANA_RESOURCE_HPP

#include "engine/common/buffer.hpp"

namespace mana {
    /**
     * Interface for resource types.
     *
     * A resource holds a object of type T.
     *
     * A resource is loaded when calling get and stays
     * in memory until free is called or the resource object is destroyed.
     *
     * A resource can be loaded again by calling get after free has been called.
     */
    template<typename T>
    class Resource {
    public:
        virtual ~Resource() = default;

        virtual T &get() = 0;

        virtual void free() = 0;
    };
}

#endif //MANA_RESOURCE_HPP
