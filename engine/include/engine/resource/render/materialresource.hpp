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

#ifndef MANA_MATERIALRESOURCE_HPP
#define MANA_MATERIALRESOURCE_HPP

#include "engine/resource/resource.hpp"

#include "engine/render/3d/rendermaterial.hpp"
#include "engine/render/renderdevice.hpp"

#include "engine/resource/resourcemanager.hpp"

namespace mana {
    class MaterialResource : public Resource<RenderMaterial> {
    public:
        MaterialResource(RenderDevice *device,
                         ResourceManager *manager,
                         ColorRGBA diffuse,
                         ColorRGBA ambient,
                         ColorRGBA specular,
                         ColorRGBA emissive,
                         float shininess,
                         std::string diffuseResource,
                         std::string ambientResource,
                         std::string specularResource,
                         std::string emissiveResource,
                         std::string shininessResource,
                         std::string normalResource);

        bool isLoaded() override;

        bool supportAsync() override;

        void load() override;

        void free() override;

        RenderMaterial &get() override;

        RenderMaterial &getOrThrow() override;

    private:
        RenderDevice *device;
        ResourceManager *manager;

        RenderMaterial mat;

        std::string diffuseResource;
        std::string ambientResource;
        std::string specularResource;
        std::string emissiveResource;
        std::string shininessResource;
        std::string normalResource;

        bool loaded;
    };
}

#endif //MANA_MATERIALRESOURCE_HPP
