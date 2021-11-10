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

#include <istream>
#include <mutex>

#include "engine/async/threadpool.hpp"

class ResourceBase {
public:
    virtual void load() = 0;

    virtual void unload() = 0;

    virtual bool isLoaded() = 0;
};

template<typename T>
class Resource : public ResourceBase {
public:
    virtual T &getResource() = 0;
};

namespace engine {
    class ResourceManager {
    public:
        ResourceManager();

        ~ResourceManager();

        template<typename T>
        T &getResource(const std::string &name) {
            std::lock_guard<std::mutex> guard(mutex);
            if (refCount.at(name) == 0)
                throw std::runtime_error("Resource reference count was not incremented for " + name);

            auto resIt = resources.find(name);
            if (resIt != resources.end()) {
                return dynamic_cast<Resource<T> &>(resIt->second).getResource();
            }

            auto taskIt = tasks.find(name);
            if (taskIt != tasks.end()) {
                taskIt->second->wait();
            }

            return dynamic_cast<Resource<T> &>(resources.at(name)).getResource();
        }

        template<typename T>
        void addResource(const std::string &name, Resource<T> &resource) {
            std::lock_guard<std::mutex> guard(mutex);
            if (resources.find(name) != resources.end())
                throw std::runtime_error("Resource with name " + name + " already exists");
            resources[name] = resource;
            refCount[name] = 0;
        }

        void removeResource(const std::string &name) {
            std::lock_guard<std::mutex> guard(mutex);
            tasks.at(name)->wait();
            resources.erase(name);
            refCount.erase(name);
        }

        void clearResources() {

        }

        uint incrementRef(const std::string &name) {
            auto &ref = refCount.at(name);
            if (ref == std::numeric_limits<uint>::max())
                throw std::runtime_error("Reference count overflow");
            if (ref == 0) {

            }
            return ++ref;
        }

        uint decrementRef(const std::string &name) {
            auto &ref = refCount.at(name);
            if (ref == 0)
                throw std::runtime_error("Reference count underflow");
            return --ref;
        }

        uint getReferenceCount(const std::string &name) {
            return refCount.at(name);
        }

    private:
        std::mutex mutex;
        std::map<std::string, uint> refCount;
        std::map<std::string, std::reference_wrapper<ResourceBase>> resources;
        std::map<std::string, std::unique_ptr<Task>> tasks;
    };
}

#endif //MANA_RESOURCEMANAGER_HPP
