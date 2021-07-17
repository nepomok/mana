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

#ifndef MANA_RESOURCEHANDLE_HPP
#define MANA_RESOURCEHANDLE_HPP

#include <cassert>

#include "engine/resource/resourcemanager.hpp"

namespace mana {
    /**
     * A resource handle automatically manages the reference count to a resource via RAII, and wraps the
     * resource pointer retrieval and access.
     *
     * The constructor increments reference to the resource in the manager and the destructor decrements the reference.
     *
     * @tparam T
     */
    template<typename T>
    class ResourceHandle {
    public:
        ResourceHandle()
                : name(), manager() {}

        ResourceHandle(const std::string &name, ResourceManager *manager)
                : name(name), manager(manager) {
            manager->incrementRef(name);
        }

        ResourceHandle(const ResourceHandle<T> &other)
                : name(other.name), manager(other.manager) {
            manager->incrementRef(name);
        }

        ResourceHandle(ResourceHandle<T> &&other) noexcept
                : name(std::move(other.name)), manager(std::move(other.manager)) {
            other.name = "";
            other.manager = nullptr;
        }

        ~ResourceHandle() {
            if (manager != nullptr)
                manager->decrementRef(name);
        }

        ResourceHandle &operator=(const ResourceHandle<T> &other) {
            if (this != &other) {
                name = other.name;
                manager = other.manager;
                manager->incrementRef(name);
            }
            return *this;
        }

        ResourceHandle &operator=(ResourceHandle<T> &&other) noexcept {
            if (this != &other) {
                name = std::move(other.name);
                manager = std::move(other.manager);
                other.name = "";
                other.manager = nullptr;
            }
            return *this;
        }

        T &get() {
            assert(manager != nullptr);
            return manager->getResource<T>(name)->get();
        }

    private:
        std::string name;
        ResourceManager *manager;
    };
}

#endif //MANA_RESOURCEHANDLE_HPP
