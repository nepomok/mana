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

namespace mana {
    MeshBufferResource::MeshBufferResource()
            : alloc(nullptr),
              meshResource(),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {
    }

    MeshBufferResource::MeshBufferResource(RenderAllocator &alloc, MeshResource &meshResource)
            : alloc(&alloc),
              meshResource(&meshResource),
              instanceOffsets(),
              instanced(false),
              mesh(nullptr) {
    }

    MeshBufferResource::MeshBufferResource(RenderAllocator &alloc,
                                           MeshResource &meshResource,
                                           std::vector<Transform> instanceOffsets)
            : alloc(&alloc),
              meshResource(&meshResource),
              instanceOffsets(std::move(instanceOffsets)),
              instanced(true),
              mesh(nullptr) {
    }

    MeshBufferResource::~MeshBufferResource() {
        if (isLoaded)
            delete mesh;
    }

    void MeshBufferResource::load() {
        if (isLoaded)
            return;

        if (instanced)
            mesh = alloc->allocateInstancedMeshBuffer(meshResource->getMesh(), instanceOffsets);
        else
            mesh = alloc->allocateMeshBuffer(meshResource->getMesh());

        isLoaded = true;
    }

    void MeshBufferResource::free() {
        if (!isLoaded)
            return;
        delete mesh;
        mesh = nullptr;
        isLoaded = false;
    }

    MeshBuffer *MeshBufferResource::getRenderMesh() {
        if (!isLoaded)
            load();
        return mesh;
    }
}
