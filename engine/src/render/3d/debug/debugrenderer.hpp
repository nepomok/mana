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

#ifndef MANA_DEBUGRENDERER_HPP
#define MANA_DEBUGRENDERER_HPP

#include <set>

#include "engine/render/rendertarget.hpp"
#include "engine/render/renderer.hpp"
#include "engine/render/renderdevice.hpp"

#include "engine/asset/camera.hpp"

namespace engine {
    class DebugRenderer {
    public:
        explicit DebugRenderer(RenderDevice *device);

        ~DebugRenderer();

        void renderBegin(RenderTarget &target) const;

        void drawLine(Vec3f start, Vec3f end, ColorRGBA color, const Mat4f &mvp);

        void drawNormalVectors(const Camera &camera,
                               const std::vector<std::pair<Transform, Mesh>> &meshes,
                               float scale = 0.1f);

        void renderFinish();

    private:
        RenderDevice *device;
        ShaderProgram *shader;

        std::set<MeshBuffer *> meshBuffers;
    };
}

#endif //MANA_DEBUGRENDERER_HPP
