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

#include "engine/render/3d/forwardpipeline.hpp"

namespace mana {
    RenderSystem::RenderSystem(RenderTarget &scr, RenderDevice &device)
            : screenTarget(scr), ren(device, {}) {
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

        std::map<RenderComponent *, TransformComponent *> mapping;
        std::vector<RenderComponent *> renderComponents;
        for (auto &nodePointer : scene.findNodesWithComponent<RenderComponent>()) {
            auto &node = *nodePointer;
            if (!node.enabled)
                continue;

            auto &comp = node.getComponent<RenderComponent>();
            if (!comp.enabled)
                continue;

            auto &tcomp = node.getComponent<TransformComponent>();
            if (!tcomp.enabled)
                continue;

            renderComponents.emplace_back(&comp);
            mapping[&comp] = &tcomp;
        }

        std::sort(renderComponents.begin(), renderComponents.end(),
                  [](const RenderComponent *a, const RenderComponent *b) -> bool {
                      return a->renderOrder < b->renderOrder;
                  });

        for (auto *comp : renderComponents) {
            ForwardCommand unit;

            unit.transform = TransformComponent::walkTransformHierarchy(*mapping[comp]);

            unit.command.shader = &comp->shader->get();
            for (auto &m : comp->textureMapping) {
                unit.command.shader->setTexture(m.first, m.second);
            }

            for (auto *t : comp->textureBuffers) {
                unit.command.textures.emplace_back(&t->get());
            }

            for (auto *m : comp->meshBuffers) {
                unit.command.meshBuffers.emplace_back(&m->get());
            }

            unit.command.properties.enableDepthTest = comp->renderProperties.enableDepthTest;
            unit.command.properties.depthTestWrite = comp->renderProperties.depthTestWrite;
            unit.command.properties.depthTestMode = comp->renderProperties.depthTestMode;

            unit.command.properties.enableStencilTest = comp->renderProperties.enableStencilTest;
            unit.command.properties.stencilTestMask = comp->renderProperties.stencilTestMask;
            unit.command.properties.stencilMode = comp->renderProperties.stencilMode;
            unit.command.properties.stencilReference = comp->renderProperties.stencilReference;
            unit.command.properties.stencilFunctionMask = comp->renderProperties.stencilFunctionMask;
            unit.command.properties.stencilFail = comp->renderProperties.stencilFail;
            unit.command.properties.stencilDepthFail = comp->renderProperties.stencilDepthFail;
            unit.command.properties.stencilPass = comp->renderProperties.stencilPass;

            unit.command.properties.enableFaceCulling = comp->renderProperties.enableFaceCulling;
            unit.command.properties.faceCullMode = comp->renderProperties.faceCullMode;
            unit.command.properties.faceCullClockwiseWinding = comp->renderProperties.faceCullClockwiseWinding;

            unit.command.properties.enableBlending = comp->renderProperties.enableBlending;
            unit.command.properties.blendSourceMode = comp->renderProperties.blendSourceMode;
            unit.command.properties.blendDestinationMode = comp->renderProperties.blendDestinationMode;

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