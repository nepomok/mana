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

#include <algorithm>
#include <utility>

namespace engine {
    Renderer3D::Renderer3D(RenderDevice &device)
            : passes(),
              geometryBuffer(device.getAllocator()),
              compositor(device, {}) {
    }

    Renderer3D::~Renderer3D() = default;

    void Renderer3D::render(RenderTarget &target,
                            Scene &scene) {
        geometryBuffer.setSize(target.getSize());

        for (auto &pass: passOrder) {
            passes.at(pass)->render(geometryBuffer, scene);
        }

        compositor.presentLayers(target, geometryBuffer);
    }

    GeometryBuffer &Renderer3D::getGeometryBuffer() {
        return geometryBuffer;
    }

    Compositor &Renderer3D::getCompositor() {
        return compositor;
    }

    void Renderer3D::clearRenderPasses() {
        passes.clear();
        passOrder.clear();
    }
}