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

#include "../system.hpp"

#include "engine/render/3d/renderer3d.hpp"
#include "engine/render/rendertarget.hpp"

#include "engine/io/archive.hpp"

#include "engine/asset/assetimporter.hpp"

namespace mana {
    class ECS;

    class RenderSystem : public System {
    public:
        RenderSystem(RenderTarget &screenTarget, RenderDevice &device, AssetImporter &importer);

        ~RenderSystem() override = default;

        void start() override;

        void stop() override;

        void update(float deltaTime, Scene &scene) override;

        Renderer3D &getRenderer();

    private:
        RenderTarget &screenTarget;
        RenderDevice &device;
        AssetImporter &assetImporter;

        Renderer3D ren;

        Camera camera;

        /**
         * Retrieve or allocate the texture buffer for a given path.
         *
         * @param path
         * @return
         */
        TextureBuffer &getTexture(const AssetPath &path);

        TextureBuffer &getCubemap(const std::array<AssetPath, 6> &paths);

        MeshBuffer &getMesh(const AssetPath &path);

        const Material &getMaterial(const AssetPath &path);

        std::set<std::string> loadedBundles;

        std::map<AssetPath, std::shared_ptr<MeshBuffer>> meshes;
        std::map<AssetPath, std::shared_ptr<TextureBuffer>> textures;
        std::map<std::array<AssetPath, 6>, std::shared_ptr<TextureBuffer>> cubeMaps;
    };
}

#endif //MANA_RENDERSYSTEM_HPP
