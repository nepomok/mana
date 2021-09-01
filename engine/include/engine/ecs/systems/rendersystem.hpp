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

#ifndef MANA_RENDERSYSTEM_HPP
#define MANA_RENDERSYSTEM_HPP

#include <map>
#include <string>

#include "engine/ecs/system.hpp"

#include "engine/render/3d/renderer3d.hpp"
#include "engine/render/rendertarget.hpp"

#include "engine/io/archive.hpp"

#include "engine/resource/resourcemanager.hpp"

namespace mana {
    class ECS;

    class RenderSystem : public System {
    public:
        RenderSystem(RenderTarget &screenTarget, RenderDevice &device, Archive &archive);

        ~RenderSystem() override = default;

        void start() override;

        void stop() override;

        void update(float deltaTime, Scene &scene) override;

        Renderer3D &getRenderer();

    private:
        RenderTarget &screenTarget;
        RenderDevice &device;
        Archive &archive;
        ResourceManager res;

        Renderer3D ren;

        Camera camera;

        TextureBuffer &getTexture(const std::string &path);

        TextureBuffer &getCubemap(const std::array<std::string, 6> &paths);

        MeshBuffer &getMesh(const std::string &path, const std::string &name);

        Material &getMaterial(const std::string &path, const std::string &name);

        std::map<std::string, std::shared_ptr<TextureBuffer>> textures;
        std::map<std::array<std::string, 6>, std::shared_ptr<TextureBuffer>> cubeMaps;
        std::map<std::string, std::shared_ptr<MeshBuffer>> meshes;
        std::map<std::string, Material> materials;
    };
}

#endif //MANA_RENDERSYSTEM_HPP
