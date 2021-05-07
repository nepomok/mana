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

#include "engine/render/renderer3d.hpp"

namespace mana {
    ShaderResource::ShaderResource()
            : alloc(nullptr), vertexShader(nullptr), fragmentShader(nullptr), shader(nullptr) {

    }

    ShaderResource::ShaderResource(RenderAllocator &alloc, TextResource &vertexShader, TextResource &fragmentShader)
            : alloc(&alloc), vertexShader(&vertexShader), fragmentShader(&fragmentShader), shader(nullptr) {
    }

    ShaderResource::~ShaderResource() {
        if (isLoaded)
            delete shader;
    }

    void ShaderResource::load() {
        if (isLoaded)
            return;
        shader = alloc->allocateShaderProgram(vertexShader->getText(),
                                              fragmentShader->getText(),
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

    ShaderProgram *ShaderResource::getShader() {
        if (!isLoaded)
            load();
        return shader;
    }
}