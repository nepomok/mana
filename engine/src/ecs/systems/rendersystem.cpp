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
#include "engine/render/3d/passes/compositepass.hpp"
#include "engine/render/3d/passes/forwardpass.hpp"

#include "engine/asset/assetimporter.hpp"

#include "render/3d/debug/debugrenderer.hpp"

namespace engine {
    RenderSystem::RenderSystem(RenderTarget &scr, RenderDevice &device, Archive &archive, bool debugRender)
            : screenTarget(scr),
              device(device),
              idCounter(0),
              ren(device, {new ForwardPass(device),
                           new GeometryPass(device),
                           new PhongShadePass(device),
                           new CompositePass(device)}),
              archive(archive),
              debugRender(debugRender) {
    }

    void RenderSystem::start(EntityManager &entityManager) {

    }

    void RenderSystem::stop(EntityManager &entityManager) {

    }

    void RenderSystem::update(float deltaTime, EntityManager &entityManager) {
        auto &componentManager = entityManager.getComponentManager();

        std::set<std::string> activeBundles;

        std::set<AssetPath> activeMeshes;
        std::set<AssetPath> activeTextures;
        std::set<std::array<AssetPath, 6>> activeCubeMaps;

        //Get active asset bundles
        for (auto &pair : componentManager.getPool<MeshRenderComponent>()) {
            //TODO: Entity wide active flag

            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            if (!transform.enabled)
                continue;

            auto &render = pair.second;
            if (!render.enabled)
                continue;

            activeBundles.insert(render.mesh.bundle);
            activeBundles.insert(render.material.bundle);
            activeMeshes.insert(render.mesh);
        }

        for (auto &pair : componentManager.getPool<SkyboxComponent>()) {
            auto &comp = pair.second;

            for (auto &path : comp.paths)
                activeBundles.insert(path.bundle);

            activeCubeMaps.insert(comp.paths);
        }

        //Get referenced asset bundles in material.
        for (auto &pair : componentManager.getPool<MeshRenderComponent>()) {
            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            if (!transform.enabled)
                continue;

            auto &render = pair.second;
            if (!render.enabled)
                continue;

            auto &material = getBundle(render.material.bundle).getMaterial(render.material.asset);

            if (!material.diffuseTexture.empty()) {
                activeBundles.insert(material.diffuseTexture.bundle);
                activeTextures.insert(material.diffuseTexture);
            }
            if (!material.ambientTexture.empty()) {
                activeBundles.insert(material.ambientTexture.bundle);
                activeTextures.insert(material.ambientTexture);
            }
            if (!material.specularTexture.empty()) {
                activeBundles.insert(material.specularTexture.bundle);
                activeTextures.insert(material.specularTexture);
            }
            if (!material.emissiveTexture.empty()) {
                activeBundles.insert(material.emissiveTexture.bundle);
                activeTextures.insert(material.emissiveTexture);
            }
            if (!material.shininessTexture.empty()) {
                activeBundles.insert(material.shininessTexture.bundle);
                activeTextures.insert(material.shininessTexture);
            }
            if (!material.normalTexture.empty()) {
                activeBundles.insert(material.normalTexture.bundle);
                activeTextures.insert(material.normalTexture);
            }
        }

        // Retrieve image bundles from active textures
        for (auto &texture : activeTextures) {
            auto &t = getBundle(texture.bundle).getTexture(texture.asset);
            activeBundles.insert(t.image.bundle);
        }

        RenderScene scene3d;

        //Create render commands
        for (auto &pair : componentManager.getPool<MeshRenderComponent>()) {
            auto &transform = componentManager.lookup<TransformComponent>(pair.first);
            if (!transform.enabled)
                continue;

            auto &render = pair.second;
            if (!render.enabled)
                continue;

            DeferredCommand command;
            command.transform = TransformComponent::walkHierarchy(transform, entityManager);
            command.meshBuffer = &getMesh(render.mesh);

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
        for (auto &pair : componentManager.getPool<SkyboxComponent>()) {
            auto &comp = pair.second;
            scene3d.skybox = &getCubemap(comp.paths);
        }

        //Unload bundles which are not referenced anymore
        setActiveBundles(activeBundles);

        //Delete unused texture buffers
        /*  std::set<AssetPath> unused;
          for (auto &pair : textures) {
              if (activeTextures.find(pair.first) == activeTextures.end())
                  unused.insert(pair.first);
          }
          for (auto &s : unused) {
              textures.erase(s); //Deallocate texture buffer
          }
          unused.clear();

          //Delete unused mesh buffers
          for (auto &pair : meshes) {
              if (activeMeshes.find(pair.first) == activeMeshes.end())
                  unused.insert(pair.first);
          }
          for (auto &s : unused) {
              meshes.erase(s); //Deallocate mesh buffer
          }
          unused.clear();

          //Delete unused cube map texture buffers
          std::set<std::array<AssetPath, 6>> unusedCubeMaps;
          for (auto &pair : cubeMaps) {
              if (activeCubeMaps.find(pair.first) == activeCubeMaps.end())
                  unusedCubeMaps.insert(pair.first);
          }
          for (auto &s : unusedCubeMaps) {
              cubeMaps.erase(s); //Deallocate cube map texture buffer
          }
          unusedCubeMaps.clear();*/

        //Get Camera
        Camera camera;

        for (auto &pair : componentManager.getPool<CameraComponent>()) {
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
        for (auto &pair : componentManager.getPool<LightComponent>()) {
            auto &lightComponent = pair.second;

            if (!lightComponent.enabled)
                continue;

            scene3d.lights.emplace_back(lightComponent.light);
        }

        //Render
        ren.render(screenTarget, scene3d);

        if (debugRender) {
            //Render debug
            std::vector<std::pair<Transform, Mesh>> debugData;
            for (auto &pair : componentManager.getPool<MeshRenderComponent>()) {
                auto &renderComponent = pair.second;

                if (!renderComponent.enabled)
                    continue;

                auto &transformComponent = componentManager.lookup<TransformComponent>(pair.first);

                debugData.emplace_back(std::pair<Transform, Mesh>(
                        TransformComponent::walkHierarchy(transformComponent, entityManager),
                        getBundle(renderComponent.mesh.bundle)
                                .getMesh(renderComponent.mesh.asset)));
            }

            DebugRenderer renDbg(&device);
            renDbg.renderBegin(screenTarget);
            renDbg.drawNormalVectors(camera, debugData);
            renDbg.renderFinish();
        }
    }

    Renderer3D &RenderSystem::getRenderer() {
        return ren;
    }

    TextureBuffer &RenderSystem::getTexture(const AssetPath &path) {
        bool found = false;
        uint id;
        for (auto &p : paths) {
            if (p.second == path) {
                id = p.first;
                found = true;
                break;
            }
        }

        if (!found) {
            auto &texture = getBundle(path.bundle).getTexture(path.asset);
            auto &image = getBundle(texture.image.bundle).getImage(texture.image.asset);

            if (!idBin.empty()) {
                id = *idBin.begin();
                idBin.erase(0);
            } else {
                if (idCounter == std::numeric_limits<uint>::max())
                    throw std::runtime_error("Overflow");
                id = idCounter++;
            }

            textures[id] = std::shared_ptr<TextureBuffer>(
                    device.getAllocator().createTextureBuffer(texture.attributes)
            );

            textures[id]->upload(image);

            paths[id] = path;
        }

        return *textures.at(id);
    }

    TextureBuffer &RenderSystem::getCubemap(const std::array<AssetPath, 6> &paths) {
        bool found = false;
        uint id;
        for (auto &p : cubeMapPaths) {
            if (p.second == paths) {
                id = p.first;
                found = true;
                break;
            }
        }

        if (!found) {
            TextureBuffer *buffer = device.getAllocator().createTextureBuffer({});
            for (int i = TextureBuffer::CubeMapFace::POSITIVE_X; i <= TextureBuffer::CubeMapFace::NEGATIVE_Z; i++) {
                auto path = paths.at(i);
                auto &image = getBundle(path.bundle);

                if (path.asset.empty()) {
                    buffer->upload(static_cast<TextureBuffer::CubeMapFace>(i), image.images.begin()->second);
                } else {
                    buffer->upload(static_cast<TextureBuffer::CubeMapFace>(i), image.images.at(path.asset));
                }
            }

            if (!idBin.empty()) {
                id = *idBin.begin();
                idBin.erase(0);
            } else {
                if (idCounter == std::numeric_limits<uint>::max())
                    throw std::runtime_error("Overflow");
                id = idCounter++;
            }
            cubeMaps[id] = std::shared_ptr<TextureBuffer>(buffer);
            cubeMapPaths[id] = paths;
        }

        return *cubeMaps.find(id)->second;
    }

    MeshBuffer &RenderSystem::getMesh(const AssetPath &path) {
        bool found = false;
        uint id;
        for (auto &p : paths) {
            if (p.second == path) {
                id = p.first;
                found = true;
                break;
            }
        }

        if (!found) {
            auto bundle = getBundle(path.bundle);

            Mesh *mesh;
            if (path.asset.empty()) {
                if (bundle.meshes.empty())
                    throw std::runtime_error("");
                mesh = &bundle.meshes.begin()->second;
            } else {
                mesh = &bundle.meshes.at(path.asset);
            }

            if (!idBin.empty()) {
                id = *idBin.begin();
                idBin.erase(idBin.begin());
            } else if (idCounter == std::numeric_limits<uint>::max()) {
                throw std::runtime_error("Cannot allocate mesh, id overflow");
            } else {
                id = idCounter++;
            }

            meshes[id] = std::shared_ptr<MeshBuffer>(device.getAllocator().createMeshBuffer(*mesh));
            paths[id] = path;
        }

        return *meshes[id];
    }

    const Material &RenderSystem::getMaterial(const AssetPath &path) {
        return getBundle(path.bundle).getMaterial(path.asset);
    }

    const AssetBundle &RenderSystem::getBundle(const std::string &bundle) {
        mutex.lock();
        auto it = bundles.find(bundle);
        if (it != bundles.end()) {
            auto &ret = it->second;
            mutex.unlock();
            return ret;
        }
        auto itl = loadingBundles.find(bundle);
        if (itl == loadingBundles.end()) {
            mutex.unlock();
            loadBundle(bundle);
            mutex.lock();
        }
        itl = loadingBundles.find(bundle);
        if (itl != loadingBundles.end()) {
            auto ptrCopy = itl->second;
            mutex.unlock();
            ptrCopy->wait();
            mutex.lock();
        }
        auto &ret = bundles.at(bundle);
        mutex.unlock();
        return ret;
    }

    void RenderSystem::loadBundle(const std::string &bundle) {
        std::lock_guard<std::mutex> guard(mutex);

        if (bundles.find(bundle) != bundles.end()
            || loadingBundles.find(bundle) != loadingBundles.end())
            return;

        loadingBundles.insert({bundle, ThreadPool::pool.addTask([this, bundle]() {
            auto b = AssetImporter::import(bundle, archive);
            std::lock_guard<std::mutex> guard(mutex);
            loadingBundles.erase(bundle);
            bundles[bundle] = b;
        })});
    }

    void RenderSystem::setActiveBundles(const std::set<std::string> &active) {
        for (auto &a : active)
            loadBundle(a);
        mutex.lock();
        std::set<std::shared_ptr<Task>> tasks;
        for (auto &bundle : loadingBundles) {
            if (active.find(bundle.first) == active.end()) {
                tasks.insert(bundle.second);
            }
        }
        mutex.unlock();
        for (auto &task : tasks)
            task->wait();
        mutex.lock();
        std::set<std::string> delBundle;
        for (auto &bundle : bundles) {
            if (active.find(bundle.first) == active.end()) {
                delBundle.insert(bundle.first);
            }
        }
        for (auto &del : delBundle) {
            bundles.erase(del);
        }
        mutex.unlock();
    }
}