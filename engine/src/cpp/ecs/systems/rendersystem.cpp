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

#include "engine/ecs/systems/rendersystem.hpp"

#include "engine/ecs/components.hpp"

#include "engine/render/3d/passes/forwardpass.hpp"

namespace mana {
    RenderSystem::RenderSystem(RenderTarget &scr, RenderDevice &device)
            : screenTarget(scr), ren(device, {new ForwardPass()}) {
    }

    void RenderSystem::start() {

    }

    void RenderSystem::stop() {

    }

    void RenderSystem::update(float deltaTime, Scene &scene) {
        RenderScene scene3d;

        for (auto *nodePointer : scene.findNodesWithComponent<LightComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto lightComponent = node.getComponent<LightComponent>();
            if (!lightComponent.enabled)
                continue;

            scene3d.lights.emplace_back(lightComponent.light);
        }

        for (auto &nodePointer : scene.findNodesWithComponent<MeshComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &transformComponent = node.getComponent<TransformComponent>();
            if (!transformComponent.enabled)
                continue;

            auto &meshComponent = node.getComponent<MeshComponent>();
            if (!meshComponent.enabled)
                continue;

            auto &materialComponent = node.getComponent<MaterialComponent>();
            if (!materialComponent.enabled)
                continue;

            RenderUnit unit;

            unit.transform = TransformComponent::walkTransformHierarchy(transformComponent);
            unit.material = &materialComponent.material->get();
            unit.mesh = &meshComponent.mesh->get();

            scene3d.forward.emplace_back(unit);
        }

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