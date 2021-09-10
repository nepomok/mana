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

#include "engine/render/3d/renderer3d.hpp"

#include "hlslinject.hpp"

#include <algorithm>
#include <utility>

namespace engine {
    std::string includeCallback(const char *n) {
        std::string name(n);
        if (name == "mana.hlsl") {
            return SHADER_MANA;
        } else {
            throw std::runtime_error("Invalid name: " + name);
        }
    }

    const std::map<std::string, std::string> gMacros = {{"MANA_MAX_LIGHTS", "20"}};

    const std::function<std::string(const char *)> gIncludeFunc = {includeCallback};

    const std::map<std::string, std::string> &Renderer3D::getShaderMacros() {
        return gMacros;
    }

    const std::function<std::string(const char *)> &Renderer3D::getShaderIncludeCallback() {
        return gIncludeFunc;
    }

    Renderer3D::Renderer3D(RenderDevice &device, std::vector<RenderPass *> passes)
            : device(&device),
              passes(std::move(passes)),
              geometryBuffer(device.getAllocator()) {
        for (auto &pass : this->passes) {
            pass->prepareBuffer(geometryBuffer);
        }
    }

    Renderer3D::~Renderer3D() = default;

    void Renderer3D::render(RenderTarget &target,
                            RenderScene &scene) {
        if (device == nullptr)
            throw std::runtime_error("Renderer 3d not initialized");

        geometryBuffer.setSize(target.getSize());

        for (auto *pass : passes) {
            pass->render(geometryBuffer, scene);
        }

        for (auto *pass : passes) {
            pass->presentBuffer(target, geometryBuffer);
        }
    }

    GeometryBuffer &Renderer3D::getGeometryBuffer() {
        return geometryBuffer;
    }
}