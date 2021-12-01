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

#include "engine/ecs/components/meshrendercomponent.hpp"
#include "engine/ecs/components/skyboxcomponent.hpp"

#include "engine/render/3d/renderer3d.hpp"
#include "engine/display/window.hpp"

#include "engine/io/archive.hpp"

#include "engine/asset/assetimporter.hpp"

namespace engine {
    class ECS;

    class DebugPass;

    class RenderSystem : public System,
                         ComponentPool<MeshRenderComponent>::Listener,
                         ComponentPool<SkyboxComponent>::Listener {
    public:
        RenderSystem(Window &window, Archive &archive, const std::set<RenderPass*>& passes);

        ~RenderSystem() override;

        void start(EntityManager &entityManager) override;

        void stop(EntityManager &entityManager) override;

        void update(float deltaTime, EntityManager &entityManager) override;

        Renderer3D &getRenderer();

        template<typename T>
        T &getRenderPass() {
            return ren->getRenderPass<T>();
        }

    private:
        void onComponentCreate(const Entity &entity, const MeshRenderComponent &component) override;

        void onComponentDestroy(const Entity &entity, const MeshRenderComponent &component) override;

        void onComponentCreate(const Entity &entity, const SkyboxComponent &component) override;

        void onComponentDestroy(const Entity &entity, const SkyboxComponent &component) override;

        void onComponentUpdate(const Entity &entity,
                               const MeshRenderComponent &oldValue,
                               const MeshRenderComponent &newValue) override;

        void onComponentUpdate(const Entity &entity,
                               const SkyboxComponent &oldValue,
                               const SkyboxComponent &newValue) override;

        RenderTarget &screenTarget;
        RenderDevice &device;
        Archive &archive;

        std::unique_ptr<Renderer3D> ren;

        /**
         * Retrieve or allocate the texture buffer for a given path.
         *
         * @param path
         * @return
         */
        TextureBuffer &getTexture(const AssetPath &path);

        TextureBuffer &getCubemap(const std::array<AssetPath, 6> &paths);

        MeshBuffer &getMeshBuffer(const AssetPath &path);

        const Material &getMaterial(const AssetPath &path);

        void loadMaterial(const AssetPath &path);

        void loadTexture(const AssetPath &path);

        void loadCubeMap(const std::array<AssetPath, 6> &paths);

        void loadMeshBuffer(const AssetPath &path);

        void unloadMaterial(const AssetPath &path);

        void unloadTexture(const AssetPath &path);

        void unloadCubeMap(const std::array<AssetPath, 6> &paths);

        void unloadMeshBuffer(const AssetPath &path);

        void loadBundle(const std::string &bundle);

        void unloadBundle(const std::string &bundle);

        std::unordered_map<std::string, ulong> bundlesRef; //The reference count for bundles (bundle -> asset)
        std::unordered_map<std::string, std::shared_ptr<Task>> bundleTasks;

        std::mutex mutex;
        std::unordered_map<std::string, AssetBundle> bundles; //Written to by bundle worker tasks and the main thread.

        std::unordered_map<AssetPath, ulong, AssetPath::Hash> assetRef; //The reference count for assets (asset -> buffer)
        std::unordered_map<AssetPath, std::unique_ptr<TextureBuffer>, AssetPath::Hash> textures;
        std::unordered_map<AssetPath, std::unique_ptr<MeshBuffer>, AssetPath::Hash> meshBuffers;

        std::unordered_map<std::array<AssetPath, 6>, ulong, AssetPath::HashArray<6>> cubeMapRef;
        std::unordered_map<std::array<AssetPath, 6>, std::unique_ptr<TextureBuffer>, AssetPath::HashArray<6>> cubeMaps;
    };
}

#endif //MANA_RENDERSYSTEM_HPP
