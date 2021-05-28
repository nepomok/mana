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

/*
#include "engine/resource/render/shaderresource.hpp"

#include "engine/render/3d/renderer3d.hpp"

namespace mana {
    ShaderResource::ShaderResource()
            : device(nullptr), vertexShader(nullptr), fragmentShader(nullptr), shader(nullptr) {

    }

    ShaderResource::ShaderResource(RenderDevice &device, Resource<std::string> &vertexShader,
                                   Resource<std::string> &fragmentShader)
            : device(&device), vertexShader(&vertexShader), fragmentShader(&fragmentShader), shader(nullptr) {
    }

    ShaderResource::~ShaderResource() {
        if (isLoaded)
            delete shader;
    }

    void ShaderResource::load() {
        if (isLoaded)
            return;
        shader = device->createShaderProgram(vertexShader->get(),
                                               fragmentShader->get(),
                                               Renderer3D::getShaderMacros(),
                                               Renderer3D::getShaderIncludeCallback());
        isLoaded = true;
    }

    void ShaderResource::free() {
        if (!isLoaded)
            return;
        delete shader;
        shader = nullptr;
        isLoaded = false;
    }

    ShaderProgram &ShaderResource::get() {
        if (!isLoaded)
            load();
        return *shader;
    }
}
 */