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

#include "resourcemanager.hpp"

namespace engine {
    template<typename T>
    class ResourceHandle {
    public:
        explicit ResourceHandle(ResourceManager &resourceManager, std::string resourceName = {})
                : resourceManager(resourceManager), resourceName(std::move(resourceName)) {
            if (!resourceName.empty())
                resourceManager.incrementRef(resourceName);
        }

        ~ResourceHandle() {
            if (!resourceName.empty())
                resourceManager.decrementRef(resourceName);
        }

        ResourceHandle(const ResourceHandle<T> &other) {
            resourceManager = other.resourceManager;
            resourceName = other.resourceName;
            if (!resourceName.empty())
                resourceManager.incrementRef(resourceName);
        }

        ResourceHandle<T> &operator=(const ResourceHandle<T> &other) {
            *this = ResourceHandle<T>(other);
            return *this;
        }

        ResourceHandle(ResourceHandle<T> &&other) noexcept {
            resourceManager = other.resourceManage;
            resourceName = std::move(other.resourceName);
        }

        ResourceHandle<T> &operator=(ResourceHandle<T> &&other) noexcept {
            resourceManager = other.resourceManage;
            resourceName = std::move(other.resourceName);
            return *this;
        }

        T &get() {
            return resourceManager.getResource<T>(resourceName);
        }

    private:
        ResourceManager &resourceManager;
        std::string resourceName;
    };
}

#endif //MANA_RESOURCEHANDLE_HPP
