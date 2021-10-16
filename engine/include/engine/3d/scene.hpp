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

#ifndef MANA_SCENE_HPP
#define MANA_SCENE_HPP

#include "engine/3d/deferredcommand.hpp"
#include "engine/3d/forwardcommand.hpp"

#include "engine/asset/camera.hpp"
#include "engine/asset/light.hpp"

namespace engine {
    struct Scene {
        Camera camera;

        std::vector<Light> lights;

        std::vector<DeferredCommand> deferred; //The commands to draw in the deferred pipeline

        // The commands to draw in the forward pipeline,
        // the forward pipeline is executed after the deferred pipeline.
        // The depth information from the deferred pass is preserved in the depth buffer.
        std::vector<ForwardCommand> forward;

        TextureBuffer* skybox = nullptr; //The cubemap texture of the skybox if the skyboxColor is drawn.
        ColorRGBA skyboxColor = ColorRGBA(0,0, 255, 255);
    };
}

#endif //MANA_SCENE_HPP
