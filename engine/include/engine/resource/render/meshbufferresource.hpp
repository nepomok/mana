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

#ifndef MANA_MESHBUFFERRESOURCE_HPP
#define MANA_MESHBUFFERRESOURCE_HPP

#include "engine/resource/resource.hpp"
#include "engine/render/renderallocator.hpp"

namespace mana {
    class MeshBufferResource : public Resource<MeshBuffer> {
    public:
        MeshBufferResource();

        MeshBufferResource(RenderAllocator &alloc, Resource <Mesh> &meshResource);

        MeshBufferResource(RenderAllocator &alloc,
                           Resource <Mesh> &meshResource,
                           std::vector<Transform> instanceOffsets);

        ~MeshBufferResource() override;

        void load() override;

        void free() override;

        MeshBuffer &get() override;

    private:
        RenderAllocator *alloc;
        Resource <Mesh> *meshResource;
        std::vector<Transform> instanceOffsets;
        bool instanced;
        MeshBuffer *mesh;
        bool isLoaded = false;
    };
}

#endif //MANA_MESHBUFFERRESOURCE_HPP
