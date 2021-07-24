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

#include "engine/resource/render/meshbufferresource.hpp"

#include <utility>

#include "engine/io/asset.hpp"

namespace mana {
    MeshBufferResource::MeshBufferResource()
            : device(nullptr),
              assetResource(),
              manager(nullptr),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::MeshBufferResource(RenderDevice &alloc,
                                           std::string assetResource,
                                           std::string meshName,
                                           ResourceManager *manager)
            : device(&alloc),
              assetResource(std::move(assetResource)),
              manager(manager),
              meshName(std::move(meshName)),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::MeshBufferResource(RenderDevice &alloc,
                                           std::string assetResource,
                                           std::string meshName,
                                           ResourceManager *manager,
                                           std::vector<Transform> instanceOffsets)
            : device(&alloc),
              assetResource(std::move(assetResource)),
              manager(manager),
              meshName(std::move(meshName)),
              instanceOffsets(std::move(instanceOffsets)),
              instanced(false),
              mesh(nullptr) {}

    MeshBufferResource::~MeshBufferResource() {
        MeshBufferResource::free();
    }

    void MeshBufferResource::load() {
        if (loaded)
            return;

        manager->incrementRef(assetResource);

        auto *assetRes = manager->getResource<Asset>(assetResource);

        if (instanced)
            mesh = device->createInstancedMeshBuffer(assetRes->get().getMeshes().at(meshName), instanceOffsets);
        else
            mesh = device->createMeshBuffer(assetRes->get().getMeshes().at(meshName));

        loaded = true;
    }

    void MeshBufferResource::free() {
        if (!loaded)
            return;
        delete mesh;
        mesh = nullptr;
        manager->decrementRef(assetResource);
        loaded = false;
    }

    MeshBuffer &MeshBufferResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return *mesh;
    }

    bool MeshBufferResource::isLoaded() {
        return loaded;
    }

    bool MeshBufferResource::supportAsync() {
        return false;
    }

    MeshBuffer &MeshBufferResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}