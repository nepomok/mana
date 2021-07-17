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

#ifndef MANA_RESOURCEMANAGER_HPP
#define MANA_RESOURCEMANAGER_HPP

#include <map>

#include "engine/resource/resource.hpp"

namespace mana {

    /**
     * The resource manager orchestrates the loading / unloading of resource objects based on
     * reference counting.
     *
     * All member methods have to be called from the same thread.
     */
    class ResourceManager {
    public:
        ResourceManager();

        explicit ResourceManager(const std::map<std::string, ResourceBase *> &resources);

        void addResource(const std::string &name, ResourceBase *resource);

        void eraseResource(const std::string &name);

        void incrementRef(const std::string &name);

        void decrementRef(const std::string &name);

        ResourceBase *getResourceBase(const std::string &name) const;

        template<typename T>
        Resource<T> *getResource(const std::string &name) const {
            return dynamic_cast<Resource<T> *>(getResourceBase(name));
        }

    private:
        std::map<std::string, ResourceBase *> resources;
        std::map<std::string, uint32_t> references;
    };
}

#endif //MANA_RESOURCEMANAGER_HPP