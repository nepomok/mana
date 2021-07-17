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

#include <limits>

#include "engine/resource/resourcemanager.hpp"

namespace mana {
    ResourceManager::ResourceManager() = default;

    ResourceManager::ResourceManager(const std::map<std::string, ResourceBase *> &resources) {
        for (auto &pair : resources)
            addResource(pair.first, pair.second);
    }

    void ResourceManager::addResource(const std::string &name, ResourceBase *resource) {
        if (resources.find(name) != resources.end())
            throw std::runtime_error("Resource with name " + name + " already exists.");
        resources[name] = resource;
        references[name] = 0;
    }

    void ResourceManager::eraseResource(const std::string &name) {
        resources.erase(name);
        references.erase(name);
    }

    void ResourceManager::incrementRef(const std::string &name) {
        auto *res = resources.at(name);
        auto &ref = references.at(name);

        if (ref == std::numeric_limits<uint32_t>::max())
            throw std::runtime_error("Reference increment overflow");

        if (ref == 0 && !res->isLoaded())
            res->load();

        ref++;
    }

    void ResourceManager::decrementRef(const std::string &name) {
        auto *res = resources.at(name);
        auto &ref = references.at(name);

        if (ref == 0)
            throw std::runtime_error("Reference decrement underflow.");

        if (ref == 1 && !res->isLoaded())
            res->free();

        ref--;
    }

    ResourceBase *ResourceManager::getResourceBase(const std::string &name) const {
        return resources.at(name);
    }
}