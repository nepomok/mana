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
#include "engine/math/rotation.hpp"

#include "hlslinject.hpp"

#include <algorithm>
#include <utility>

namespace mana {
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

    Renderer3D::Renderer3D() : device(nullptr) {};

    Renderer3D::Renderer3D(RenderDevice &device, std::vector<RenderPass *> passes) : device(&device),
                                                                                     passes(std::move(passes)) {}

    Renderer3D::~Renderer3D() {
        for (auto *pass : passes)
            delete pass;
    }

    void Renderer3D::setRenderDevice(RenderDevice *dev) {
        device = dev;
    }

    const RenderDevice &Renderer3D::getRenderDevice() {
        if (device == nullptr)
            throw std::runtime_error("Renderer3D not initialized");
        return *device;
    }

    void Renderer3D::setRenderPasses(std::vector<RenderPass *> p) {
        passes = std::move(p);
    }

    const std::vector<RenderPass *> &Renderer3D::getRenderPasses() {
        return passes;
    }

    void Renderer3D::render(RenderTarget &target,
                            RenderScene &scene) {
        if (device == nullptr)
            throw std::runtime_error("Renderer 3d not initialized");

        for (auto *pass : passes) {
            pass->render(*device, target, scene);
        }
    }
}