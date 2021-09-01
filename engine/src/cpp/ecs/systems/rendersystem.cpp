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

#include "engine/render/3d/forwardpipeline.hpp"
#include "engine/render/3d/passes/geometrypass.hpp"
#include "engine/render/3d/passes/lightingpass.hpp"

#include "engine/asset/assetimporter.hpp"

namespace mana {
    RenderSystem::RenderSystem(RenderTarget &scr, RenderDevice &device, Archive &archive)
            : screenTarget(scr),
              device(device),
              ren(device, {new GeometryPass(device), new LightingPass(device)}),
              archive(archive),
              res(archive) {
    }

    void RenderSystem::start() {

    }

    void RenderSystem::stop() {

    }

    void RenderSystem::update(float deltaTime, Scene &scene) {
        RenderScene scene3d;

        std::set<std::string> usedTextures;
        std::set<std::string> usedMeshes;
        std::set<std::string> usedMaterials;

        //Get lights
        for (auto *nodePointer : scene.findNodesWithComponent<LightComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto lightComponent = node.getComponent<LightComponent>();
            if (!lightComponent.enabled)
                continue;

            scene3d.lights.emplace_back(lightComponent.light);
        }

        //Get render commands
        for (auto &nodePointer : scene.findNodesWithComponent<RenderComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &comp = node.getComponent<RenderComponent>();
            if (!comp.enabled)
                continue;

            if (!node.hasComponent<TransformComponent>())
                continue;
            auto &tcomp = node.getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            if (!node.hasComponent<MaterialComponent>())
                continue;
            auto &matcomp = node.getComponent<MaterialComponent>();
            if (!matcomp.enabled)
                continue;

            if (!node.hasComponent<MeshComponent>())
                continue;
            auto &meshcomp = node.getComponent<MeshComponent>();
            if (!meshcomp.enabled)
                continue;

            usedMaterials.insert(matcomp.path + "/" + matcomp.name);
            usedMeshes.insert(meshcomp.path + "/" + meshcomp.name);

            if (comp.forward) {
            } else {
                auto &material = getMaterial(matcomp.path, matcomp.name);

                if (!material.diffuseTexture.empty()) {
                    usedTextures.insert(material.diffuseTexture);
                }
                if (!material.ambientTexture.empty()) {
                    usedTextures.insert(material.ambientTexture);
                }
                if (!material.specularTexture.empty()) {
                    usedTextures.insert(material.specularTexture);
                }
                if (!material.emissiveTexture.empty()) {
                    usedTextures.insert(material.emissiveTexture);
                }
                if (!material.shininessTexture.empty()) {
                    usedTextures.insert(material.shininessTexture);
                }
                if (!material.normalTexture.empty()) {
                    usedTextures.insert(material.normalTexture);
                }
            }
        }

        //Get Skybox
        for (auto &node : scene.findNodesWithComponent<SkyboxComponent>()) {
            auto &comp = node->getComponent<SkyboxComponent>();
            usedTextures.insert(comp.paths.begin(), comp.paths.end());
        }

        //Load used resources
        for (auto &s : usedTextures) {
            res.loadImage(s);
        }

        //Get render commands
        for (auto &nodePointer : scene.findNodesWithComponent<RenderComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &comp = node.getComponent<RenderComponent>();
            if (!comp.enabled)
                continue;

            if (!node.hasComponent<TransformComponent>())
                continue;
            auto &tcomp = node.getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            if (!node.hasComponent<MaterialComponent>())
                continue;
            auto &matcomp = node.getComponent<MaterialComponent>();
            if (!matcomp.enabled)
                continue;

            if (!node.hasComponent<MeshComponent>())
                continue;
            auto &meshcomp = node.getComponent<MeshComponent>();
            if (!meshcomp.enabled)
                continue;

            if (comp.forward) {
                /*ForwardCommand unit;
                unit.transform = TransformComponent::walkTransformHierarchy(tcomp);
                scene3d.forward.emplace_back(unit);*/
            } else {
                DeferredCommand command;
                command.transform = TransformComponent::walkTransformHierarchy(tcomp);

                auto &material = getMaterial(matcomp.path, matcomp.name);

                RenderMaterial renderMaterial;
                renderMaterial.diffuse = material.diffuse;
                renderMaterial.ambient = material.ambient;
                renderMaterial.specular = material.specular;
                renderMaterial.emissive = material.emissive;
                renderMaterial.shininess = material.shininess;

                if (!material.diffuseTexture.empty()) {
                    renderMaterial.diffuseTexture = &getTexture(material.diffuseTexture);
                }
                if (!material.ambientTexture.empty()) {
                    renderMaterial.ambientTexture = &getTexture(material.ambientTexture);
                }
                if (!material.specularTexture.empty()) {
                    renderMaterial.specularTexture = &getTexture(material.specularTexture);
                }
                if (!material.emissiveTexture.empty()) {
                    renderMaterial.emissiveTexture = &getTexture(material.emissiveTexture);
                }
                if (!material.shininessTexture.empty()) {
                    renderMaterial.shininessTexture = &getTexture(material.shininessTexture);
                }
                if (!material.normalTexture.empty()) {
                    renderMaterial.normalTexture = &getTexture(material.normalTexture);
                }

                command.material = renderMaterial;
                command.meshBuffer = &getMesh(meshcomp.path, meshcomp.name);
                command.outline = comp.outline;
                command.outlineColor = comp.outlineColor;
                command.outlineScale = comp.outlineScale;

                scene3d.deferred.emplace_back(command);
            }
        }

        std::set<std::array<std::string, 6>> usedCubemaps;

        //Get Skybox
        for (auto &node : scene.findNodesWithComponent<SkyboxComponent>()) {
            auto &comp = node->getComponent<SkyboxComponent>();

            usedCubemaps.insert(comp.paths);

            scene3d.skybox = &getCubemap(comp.paths);
        }

        //Get Camera
        for (auto &node : scene.findNodesWithComponent<CameraComponent>()) {
            if (!node->enabled)
                continue;
            auto &comp = node->getComponent<CameraComponent>();
            if (!comp.enabled)
                continue;
            auto &tcomp = node->getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            scene3d.camera = comp.camera;
            scene3d.camera.transform = TransformComponent::walkTransformHierarchy(tcomp);

            break;
        }

        //Delete and unload unused resources
        std::set<std::string> unused;
        for (auto &pair : textures) {
            if (usedTextures.find(pair.first) == usedTextures.end())
                unused.insert(pair.first);
        }
        for (auto &s : unused) {
            textures.erase(s);
            res.unload(s); //Image not used in any texture buffer cubemap or 2d
        }
        unused.clear();

        for (auto &pair : meshes) {
            if (usedMeshes.find(pair.first) == usedMeshes.end())
                unused.insert(pair.first);
        }
        for (auto &s : unused) {
            meshes.erase(s);
        }
        unused.clear();

        for (auto &pair : materials) {
            if (usedMaterials.find(pair.first) == usedMaterials.end())
                unused.insert(pair.first);
        }
        for (auto &s : unused) {
            materials.erase(s);
        }
        unused.clear();

        std::set<std::array<std::string, 6>> unusedCubemaps;
        for (auto &pair : cubeMaps) {
            if (usedCubemaps.find(pair.first) == usedCubemaps.end())
                unusedCubemaps.insert(pair.first);
        }
        for (auto &s : unusedCubemaps) {
            cubeMaps.erase(s); //Erase cube map texture buffer instances which are no longer referenced.
        }
        unusedCubemaps.clear();

        //Render
        ren.render(screenTarget, scene3d);
    }

    Renderer3D &RenderSystem::getRenderer() {
        return ren;
    }

    TextureBuffer &RenderSystem::getTexture(const std::string &path) {
        if (textures.find(path) == textures.end()) {
            auto &image = res.getImage(path);
            textures[path] = std::shared_ptr<TextureBuffer>(device.getAllocator().createTextureBuffer({}));
            textures[path]->upload(image);
        }
        return *textures.find(path)->second;
    }

    TextureBuffer &RenderSystem::getCubemap(const std::array<std::string, 6> &paths) {
        if (cubeMaps.find(paths) == cubeMaps.end()) {
            TextureBuffer *buffer = device.getAllocator().createTextureBuffer({});
            for (int i = TextureBuffer::CubeMapFace::POSITIVE_X; i <= TextureBuffer::CubeMapFace::NEGATIVE_Z; i++) {
                auto &image = res.getImage(paths.at(i));
                buffer->upload(static_cast<TextureBuffer::CubeMapFace>(i), image);
            }
            cubeMaps[paths] = std::shared_ptr<TextureBuffer>(buffer);
        }
        return *cubeMaps.find(paths)->second;
    }

    MeshBuffer &RenderSystem::getMesh(const std::string &path, const std::string &name) {
        std::string key = path + "/" + name;
        if (meshes.find(key) == meshes.end()) {
            auto scene = AssetImporter::importAssetScene(*archive.open(path), std::filesystem::path(path).extension());

            Mesh *mesh;
            if (name.empty()) {
                if (scene.meshes.empty())
                    throw std::runtime_error("");
                mesh = &scene.meshes.begin()->second;
            } else {
                mesh = &scene.meshes.at(name);
            }

            meshes[key] = std::shared_ptr<MeshBuffer>(device.getAllocator().createMeshBuffer(*mesh));
        }
        return *meshes[key];
    }

    Material &RenderSystem::getMaterial(const std::string &path, const std::string &name) {
        std::string key = path + "/" + name;
        if (materials.find(key) == materials.end()) {
            auto scene = AssetImporter::importAssetScene(*archive.open(path), std::filesystem::path(path).extension());

            Material *material;
            if (name.empty()) {
                if (scene.materials.empty())
                    throw std::runtime_error("");
                material = &scene.materials.begin()->second;
            } else {
                material = &scene.materials.at(name);
            }

            materials[key] = *material;
        }

        return materials[key];
    }
}