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
              archive(archive) {
    }

    void RenderSystem::start() {

    }

    void RenderSystem::stop() {

    }

    void RenderSystem::update(float deltaTime, Scene &scene) {
        RenderScene scene3d;

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

            if (comp.forward) {
                /*ForwardCommand unit;
                unit.transform = TransformComponent::walkTransformHierarchy(tcomp);
                scene3d.forward.emplace_back(unit);*/
            } else {
                /*DeferredCommand command;
                command.transform = TransformComponent::walkTransformHierarchy(tcomp);
                command.material = comp.material.get();
                command.meshBuffer = &comp.meshBuffer.get();
                command.outline = comp.outline;
                command.outlineColor = comp.outlineColor;
                command.outlineScale = comp.outlineScale;
                scene3d.deferred.emplace_back(command);*/
            }
        }

        //Get Camera
        Node *cameraNode;
        for (auto &node : scene.findNodesWithComponent<CameraComponent>()) {
            if (!node->enabled)
                continue;
            auto &comp = node->getComponent<CameraComponent>();
            if (!comp.enabled)
                continue;
            auto &tcomp = node->getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;
            cameraNode = node;
            break;
        }

        //Get Skybox
        for (auto &node : scene.findNodesWithComponent<SkyboxComponent>()) {
            auto &comp = node->getComponent<SkyboxComponent>();

            if (comp.userData == nullptr) {
                auto *stream = archive.open(comp.path);
                auto image = AssetImporter::importImage(*stream, std::filesystem::path(comp.path).extension());
                delete stream;

                TextureBuffer::Attributes attribs;
                attribs.textureType = TextureBuffer::TEXTURE_CUBE_MAP;
                attribs.size = {image.getSize().x / 6, image.getSize().y};
                auto *textureBuffer = device.getAllocator().createTextureBuffer(attribs);
                textureBuffer->uploadCubeMap(image);
                comp.userData = std::shared_ptr<void>(textureBuffer);
            }

            scene3d.skybox = reinterpret_cast<TextureBuffer *>(comp.userData.get());
        }

        auto &cameraComponent = cameraNode->getComponent<CameraComponent>();
        auto &cameraTransformComponent = cameraNode->getComponent<TransformComponent>();

        scene3d.camera = cameraComponent.camera;
        scene3d.camera.transform = TransformComponent::walkTransformHierarchy(cameraTransformComponent);

        ren.render(screenTarget, scene3d);
    }

    Renderer3D &RenderSystem::getRenderer() {
        return ren;
    }
}