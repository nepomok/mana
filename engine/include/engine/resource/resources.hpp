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

#ifndef MANA_RESOURCES_HPP
#define MANA_RESOURCES_HPP

#include <string>
#include <map>

#include "engine/resource/imageresource.hpp"
#include "engine/resource/meshresource.hpp"
#include "engine/resource/render/meshbufferresource.hpp"
#include "engine/resource/render/shaderresource.hpp"
#include "engine/resource/textresource.hpp"

namespace mana {
    class Resources {
    public:
        std::map<std::string, Resource *> resources;

        Resources() : resources() {}

        ~Resources() {
            for (auto &p : resources) {
                delete p.second;
            }
        }

        Resources(Resources &&other) noexcept {
            resources = other.resources;
            other.resources = {};
        }

        Resources &operator=(const Resources &other) {
            throw std::runtime_error("Resources cannot be copied");
        }

        void addResource(const std::string &name, Resource *r) {
            if (resources.find(name) != resources.end())
                throw std::runtime_error("Resource with name " + name + " already exists");
            resources[name] = r;
        }

        void removeResource(const std::string &name) {
            if (resources.find(name) == resources.end())
                throw std::runtime_error("Resource with name " + name + " not found");
            resources.erase(name);
        }

        template<typename T>
        T &getResource(const std::string &name) const {
            if (resources.find(name) == resources.end())
                throw std::runtime_error("Resource with name " + name + " not found");
            return *dynamic_cast<T *>(resources.at(name));
        }
    };
}

#endif //MANA_RESOURCES_HPP
