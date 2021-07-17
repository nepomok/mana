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

#include <stdexcept>
#include <limits>

namespace mana {
    /**
     * The interface to some resource implementation.
     *
     * Subclasses may load from disk, network etc the manager does not have any requirements on how and from
     * where the resource is loaded.
     *
     * A resource may depend on another resource in which case the resource implementation increments the reference
     * to another resource it depends on by interacting with the manager responsible for the required resource.
     */
    class ResourceBase {
    public:
        virtual ~ResourceBase() = default;

        virtual bool isLoaded() = 0;

        /**
         * Return true if the load/free methods can be called from any thread other than the main thread.
         *
         * Resources such as render resources require to be loaded on the same thread that created the render device.
         *
         * @return True if load/free can be called from a non main thread.
         */
        virtual bool supportAsync() = 0;

        virtual void load() = 0;

        virtual void free() = 0;
    };

    //TODO: Implement resource synchronization
    template<typename T>
    class Resource : public ResourceBase {
    public:
        /**
         * Block until the resource is loaded.
         *
         * @return A reference to the resource data.
         */
        virtual T &get() = 0;

        /**
         * Throw if the resource is not loaded.
         *
         * @return A reference to the resource data.
         */
        virtual T &getOrThrow() = 0;
    };
}

#endif //MANA_RESOURCE_HPP
