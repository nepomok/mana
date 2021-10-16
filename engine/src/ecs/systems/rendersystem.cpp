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

#include <algorithm>
#include <filesystem>

#include "engine/ecs/systems/rendersystem.hpp"

#include "engine/ecs/components.hpp"

#include "engine/render/3d/passes/geometrypass.hpp"
#include "engine/render/3d/passes/phongshadepass.hpp"
#include "engine/render/3d/passes/forwardpass.hpp"
#include "engine/render/3d/passes/debugpass.hpp"

#include "engine/asset/assetimporter.hpp"

namespace engine {
    RenderSystem::RenderSystem(RenderTarget &scr, RenderDevice &device, Archive &archive)
            : screenTarget(scr),
              device(device),
              debugPass(new DebugPass(device)),
              ren(),
              archive(archive) {
        layers = {
                {{"skybox"},                                           "", DEPTH_TEST_ALWAYS},
                {{"phong_ambient", "phong_diffuse", "phong_specular"}, "depth"},
                {{"forward"},                                          "forward_depth"},
                {{"debug"},                                            "", DEPTH_TEST_ALWAYS}
        };
        ren = std::make_unique<Renderer3D>(device,
                                           std::vector<RenderPass *>(
                                                   {
                                                           new ForwardPass(device),
                                                           new GeometryPass(device),
                                                           new PhongShadePass(device),
                                                           debugPass
                                                   }),
                                           layers);
    }

    void RenderSystem::start(EntityManager &entityManager) {
        entityManager.getComponentManager().getPool<MeshRenderComponent>().addListener(this);
        entityManager.getComponentManager().getPool<SkyboxComponent>().addListener(this);
    }

    void RenderSystem::stop(EntityManager &entityManager) {
        entityManager.getComponentManager().getPool<MeshRenderComponent>().removeListener(this);
        entityManager.getComponentManager().getPool<SkyboxComponent>().removeListener(this);
    }

    void RenderSystem::update(float deltaTime, EntityManager &entityManager) {
        auto &componentManager = entityManager.getComponentManager();

        RenderScene scene3d;

        //Create render commands
        for (auto &pair: componentManager.getPool<MeshRenderComponent>()) {
            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            if (!transform.enabled)
                continue;

            auto &render = pair.second;
            if (!render.enabled)
                continue;

            DeferredCommand command;
            command.transform = TransformComponent::walkHierarchy(transform, entityManager);
            command.meshBuffer = &getMeshBuffer(render.mesh);

            auto &material = getMaterial(render.material);

            RenderMaterial renderMaterial;
            renderMaterial.diffuse = material.diffuse;
            renderMaterial.ambient = material.ambient;
            renderMaterial.specular = material.specular;
            renderMaterial.emissive = material.emissive;
            renderMaterial.shininess = material.shininess;

            if (!material.diffuseTexture.bundle.empty()) {
                renderMaterial.diffuseTexture = &getTexture(material.diffuseTexture);
            }
            if (!material.ambientTexture.bundle.empty()) {
                renderMaterial.ambientTexture = &getTexture(material.ambientTexture);
            }
            if (!material.specularTexture.bundle.empty()) {
                renderMaterial.specularTexture = &getTexture(material.specularTexture);
            }
            if (!material.emissiveTexture.bundle.empty()) {
                renderMaterial.emissiveTexture = &getTexture(material.emissiveTexture);
            }
            if (!material.shininessTexture.bundle.empty()) {
                renderMaterial.shininessTexture = &getTexture(material.shininessTexture);
            }
            if (!material.normalTexture.bundle.empty()) {
                renderMaterial.normalTexture = &getTexture(material.normalTexture);
            }

            command.material = renderMaterial;

            scene3d.deferred.emplace_back(command);
        }

        //Get Skybox
        for (auto &pair: componentManager.getPool<SkyboxComponent>()) {
            auto &comp = pair.second;
            scene3d.skybox = &getCubemap(comp.paths);
        }

        //Get Camera
        Camera camera;
        for (auto &pair: componentManager.getPool<CameraComponent>()) {
            auto &tcomp = componentManager.lookup<TransformComponent>(pair.first);
            if (!tcomp.enabled)
                continue;

            auto &comp = pair.second;

            scene3d.camera = comp.camera;
            scene3d.camera.transform = TransformComponent::walkHierarchy(tcomp, entityManager);

            camera = scene3d.camera;

            break;
        }

        //Get lights
        for (auto &pair: componentManager.getPool<LightComponent>()) {
            auto &lightComponent = pair.second;
            auto &tcomp = componentManager.getPool<TransformComponent>().lookup(pair.first);

            if (!lightComponent.enabled)
                continue;

            if (!tcomp.enabled)
                continue;

            lightComponent.light.transform = tcomp.transform;

            scene3d.lights.emplace_back(lightComponent.light);
        }

        //Render
        ren->render(screenTarget, scene3d);
    }

    void RenderSystem::setDrawDebugNormals(bool draw) {
        debugPass->setDrawNormals(draw);
    }

    void RenderSystem::setDrawDebugLightCasters(bool draw) {
        debugPass->setDrawLightCasters(draw);
    }

    Renderer3D &RenderSystem::getRenderer() {
        return *ren;
    }

    TextureBuffer &RenderSystem::getTexture(const AssetPath &path) {
        auto it = textures.find(path);
        if (it != textures.end())
            return *it->second;
        else {
            bundleTasks.at(path.bundle)->wait();

            auto &bundle = bundles.at(path.bundle);
            auto &tex = bundle.getTexture(path.asset);

            bundleTasks.at(tex.image.bundle)->wait();

            std::lock_guard<std::mutex> guard(mutex);
            auto &img = bundles.at(tex.image.bundle).getImage(tex.image.asset);
            textures[path] = std::unique_ptr<TextureBuffer>(device.getAllocator().createTextureBuffer(tex.attributes));
            textures[path]->upload(img);
            return *textures[path];
        }
    }

    TextureBuffer &RenderSystem::getCubemap(const std::array<AssetPath, 6> &paths) {
        auto it = cubeMaps.find(paths);
        if (it != cubeMaps.end())
            return *it->second;
        else {
            auto *tex = device.getAllocator().createTextureBuffer({TextureBuffer::TEXTURE_CUBE_MAP});
            for (TextureBuffer::CubeMapFace face = TextureBuffer::POSITIVE_X;
                 face <= TextureBuffer::NEGATIVE_Z;
                 face = static_cast<TextureBuffer::CubeMapFace>(face + 1)) {
                auto &path = paths.at(face);

                bundleTasks.at(path.bundle)->wait();

                std::lock_guard<std::mutex> guard(mutex);

                auto &bundle = bundles.at(path.bundle);
                auto &img = bundle.getImage(path.asset);

                tex->upload(face, img);
            }
            cubeMaps[paths] = std::unique_ptr<TextureBuffer>(tex);
            return *tex;
        }
    }

    MeshBuffer &RenderSystem::getMeshBuffer(const AssetPath &path) {
        auto it = meshBuffers.find(path);
        if (it != meshBuffers.end())
            return *it->second;
        else {
            bundleTasks.at(path.bundle)->wait();

            std::lock_guard<std::mutex> guard(mutex);

            auto &bundle = bundles.at(path.bundle);
            auto *mesh = device.getAllocator().createMeshBuffer(bundle.getMesh(path.asset));

            meshBuffers[path] = std::unique_ptr<MeshBuffer>(mesh);

            return *mesh;
        }
    }

    const Material &RenderSystem::getMaterial(const AssetPath &path) {
        bundleTasks.at(path.bundle)->wait();
        std::lock_guard<std::mutex> guard(mutex);
        return bundles.at(path.bundle).getMaterial(path.asset);
    }

    void RenderSystem::loadMaterial(const AssetPath &path) {
        loadBundle(path.bundle);
        auto material = getMaterial(path);
        loadTexture(material.diffuseTexture);
        loadTexture(material.ambientTexture);
        loadTexture(material.specularTexture);
        loadTexture(material.emissiveTexture);
        loadTexture(material.shininessTexture);
        loadTexture(material.normalTexture);
    }

    void RenderSystem::loadTexture(const AssetPath &path) {
        if (!path.empty()) {
            loadBundle(path.bundle);
            bundleTasks.at(path.bundle)->wait();
            std::lock_guard<std::mutex> guard(mutex);
            loadBundle(bundles.at(path.bundle)
                               .getTexture(path.asset).image.bundle);
            assetRef[path]++;
        }
    }

    void RenderSystem::loadCubeMap(const std::array<AssetPath, 6> &paths) {
        for (auto &path: paths) {
            loadBundle(path.bundle);
        }
        cubeMapRef[paths]++;
    }

    void RenderSystem::loadMeshBuffer(const AssetPath &path) {
        loadBundle(path.bundle);
        assetRef[path]++;
    }

    void RenderSystem::unloadMaterial(const AssetPath &path) {
        auto material = getMaterial(path);
        unloadTexture(material.diffuseTexture);
        unloadTexture(material.ambientTexture);
        unloadTexture(material.specularTexture);
        unloadTexture(material.emissiveTexture);
        unloadTexture(material.shininessTexture);
        unloadTexture(material.normalTexture);
        unloadBundle(path.bundle);
    }

    void RenderSystem::unloadTexture(const AssetPath &path) {
        if (!path.empty()) {
            bundleTasks.at(path.bundle)->wait();
            std::string tmp;
            {
                std::lock_guard<std::mutex> guard(mutex);
                tmp = bundles.at(path.bundle).getTexture(path.asset).image.bundle;
            }
            unloadBundle(tmp);
            unloadBundle(path.bundle);
            assetRef.at(path)--;
            if (assetRef.at(path) == 0) {
                textures.erase(path);
            }
        }
    }

    void RenderSystem::unloadCubeMap(const std::array<AssetPath, 6> &paths) {
        for (auto &path: paths) {
            unloadBundle(path.bundle);
        }
        cubeMapRef.at(paths)--;
        if (cubeMapRef.at(paths) == 0)
            cubeMaps.erase(paths);
    }

    void RenderSystem::unloadMeshBuffer(const AssetPath &path) {
        bundleTasks.at(path.bundle)->wait();
        unloadBundle(path.bundle);
        assetRef.at(path)--;
        if (assetRef.at(path) == 0)
            meshBuffers.erase(path);
    }

    void RenderSystem::loadBundle(const std::string &bundle) {
        auto &ref = bundlesRef[bundle];
        ref++;
        if (bundleTasks.find(bundle) == bundleTasks.end()) {
            bundleTasks[bundle] = ThreadPool::pool.addTask([this, bundle]() {
                auto bdl = AssetImporter::import(bundle, archive);
                std::lock_guard<std::mutex> guard(mutex);
                bundles[bundle] = bdl;
            });
        }
    }

    void RenderSystem::unloadBundle(const std::string &bundle) {
        auto ref = bundlesRef.at(bundle);
        auto task = bundleTasks.at(bundle);
        assert(ref > 0);
        ref--;
        if (ref == 0) {
            task->wait();
            std::lock_guard<std::mutex> guard(mutex);
            bundles.erase(bundle);
            bundleTasks.erase(bundle);
            bundlesRef.erase(bundle);
        }
    }

    void RenderSystem::onComponentCreate(const Entity &entity, const MeshRenderComponent &component) {
        loadMeshBuffer(component.mesh);
        loadMaterial(component.material);
    }

    void RenderSystem::onComponentDestroy(const Entity &entity, const MeshRenderComponent &component) {
        unloadMeshBuffer(component.mesh);
        unloadMaterial(component.material);
    }

    void RenderSystem::onComponentCreate(const Entity &entity, const SkyboxComponent &component) {
        loadCubeMap(component.paths);
    }

    void RenderSystem::onComponentDestroy(const Entity &entity, const SkyboxComponent &component) {
        unloadCubeMap(component.paths);
    }

    void RenderSystem::onComponentUpdate(const Entity &entity,
                                         const MeshRenderComponent &oldValue,
                                         const MeshRenderComponent &newValue) {
        if (oldValue == newValue)
            return;
        unloadMeshBuffer(oldValue.mesh);
        unloadMaterial(oldValue.material);
        loadMeshBuffer(newValue.mesh);
        loadMaterial(newValue.material);
    }

    void RenderSystem::onComponentUpdate(const Entity &entity,
                                         const SkyboxComponent &oldValue,
                                         const SkyboxComponent &newValue) {
        if (oldValue == newValue)
            return;
        unloadCubeMap(oldValue.paths);
        loadCubeMap(newValue.paths);
    }
}