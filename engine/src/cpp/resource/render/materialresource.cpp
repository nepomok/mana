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

#include "engine/resource/render/materialresource.hpp"

#include "engine/io/asset.hpp"

namespace mana {
    MaterialResource::MaterialResource(RenderDevice *device,
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
                                       std::string normalResource)
            : device(device),
              manager(manager),
              diffuseResource(std::move(diffuseResource)),
              ambientResource(std::move(ambientResource)),
              specularResource(std::move(specularResource)),
              emissiveResource(std::move(emissiveResource)),
              shininessResource(std::move(shininessResource)),
              normalResource(std::move(normalResource)),
              loaded(false) {
        mat.diffuse = diffuse;
        mat.ambient = ambient;
        mat.specular = specular;
        mat.emissive = emissive;
        mat.shininess = shininess;
    }

    bool MaterialResource::isLoaded() {
        return loaded;
    }

    bool MaterialResource::supportAsync() {
        return false;
    }

    void MaterialResource::load() {
        if (!diffuseResource.empty()) {
            manager->incrementRef(diffuseResource);
            mat.diffuseTexture = &manager->getResource<TextureBuffer>(diffuseResource)->get();
        }
        if (!ambientResource.empty()) {
            manager->incrementRef(ambientResource);
            mat.ambientTexture = &manager->getResource<TextureBuffer>(ambientResource)->get();
        }
        if (!specularResource.empty()) {
            manager->incrementRef(specularResource);
            mat.specularTexture = &manager->getResource<TextureBuffer>(specularResource)->get();
        }
        if (!emissiveResource.empty()) {
            manager->incrementRef(emissiveResource);
            mat.emissiveTexture = &manager->getResource<TextureBuffer>(emissiveResource)->get();
        }
        if (!shininessResource.empty()) {
            manager->incrementRef(shininessResource);
            mat.shininessTexture = &manager->getResource<TextureBuffer>(shininessResource)->get();
        }
        if (!normalResource.empty()) {
            manager->incrementRef(normalResource);
            mat.normalTexture = &manager->getResource<TextureBuffer>(normalResource)->get();
        }
        loaded = true;
    }

    void MaterialResource::free() {
        mat.diffuseTexture = nullptr;
        mat.ambientTexture = nullptr;
        mat.specularTexture = nullptr;
        mat.emissiveTexture = nullptr;
        mat.shininessTexture = nullptr;
        mat.normalTexture = nullptr;
        if (!diffuseResource.empty()) {
            manager->decrementRef(diffuseResource);
        }
        if (!ambientResource.empty()) {
            manager->decrementRef(ambientResource);
        }
        if (!specularResource.empty()) {
            manager->decrementRef(specularResource);
        }
        if (!emissiveResource.empty()) {
            manager->decrementRef(emissiveResource);
        }
        if (!shininessResource.empty()) {
            manager->decrementRef(shininessResource);
        }
        if (!normalResource.empty()) {
            manager->decrementRef(normalResource);
        }
        loaded = false;
    }

    RenderMaterial &MaterialResource::get() {
        if (!loaded)
            throw std::runtime_error("Resource not loaded");
        return mat;
    }

    RenderMaterial &MaterialResource::getOrThrow() {
        return get();
    }
}