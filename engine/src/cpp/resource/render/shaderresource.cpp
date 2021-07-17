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

#include "engine/resource/render/shaderresource.hpp"

#include "engine/render/3d/renderer3d.hpp"

namespace mana {
    ShaderResource::ShaderResource()
            : device(nullptr),
              vertexShader(),
              fragmentShader(),
              manager(nullptr),
              shader(nullptr) {}

    ShaderResource::ShaderResource(RenderDevice &device,
                                   std::string vertexShader,
                                   std::string fragmentShader,
                                   ResourceManager *manager)
            : device(&device),
              vertexShader(std::move(vertexShader)),
              fragmentShader(std::move(fragmentShader)),
              manager(manager),
              shader(nullptr) {}

    ShaderResource::~ShaderResource() {
        ShaderResource::free();
    }

    void ShaderResource::load() {
        if (loaded)
            return;
        manager->incrementRef(vertexShader);
        manager->incrementRef(fragmentShader);
        auto *vertexRes = manager->getResource<std::string>(vertexShader);
        auto *fragmentRes = manager->getResource<std::string>(fragmentShader);
        shader = device->createShaderProgram(vertexRes->get(),
                                             fragmentRes->get(),
                                             Renderer3D::getShaderMacros(),
                                             Renderer3D::getShaderIncludeCallback());
        loaded = true;
    }

    void ShaderResource::free() {
        if (!loaded)
            return;
        delete shader;
        shader = nullptr;
        manager->decrementRef(vertexShader);
        manager->decrementRef(fragmentShader);
        loaded = false;
    }

    ShaderProgram &ShaderResource::get() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return *shader;
    }

    bool ShaderResource::isLoaded() {
        return loaded;
    }

    bool ShaderResource::supportAsync() {
        return false;
    }

    ShaderProgram &ShaderResource::getOrThrow() {
        if (!loaded)
            throw std::runtime_error("Not loaded");
        return get();
    }
}