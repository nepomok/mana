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

#ifndef MANA_SAMPLE0_HPP
#define MANA_SAMPLE0_HPP


#include "game.hpp"

class Sample0 : public Game {
public:
    ~Sample0() override = default;

protected:
    void start(Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        Game::start(window, ren, alloc, input);
        ren.setClearColor(clearColor);
        manaAssembly = domain.loadAssembly("mana.dll");
        ecs.addSystem(new ScriptingSystem(*res, input, domain, *manaAssembly));
        ecs.addSystem(new RenderSystem(window.getRenderTarget(), ren3d, *res));
    }

    void stop(Window &window, Renderer &renderApi, RenderAllocator &alloc, Input &input) override {
        delete manaAssembly;
        Game::stop(window, renderApi, alloc, input);
    }

    void update(float deltaTime, Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        cameraNode->getComponent<CameraComponent>().aspectRatio =
                (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        window.update();

        ren.setViewport({}, window.getFramebufferSize());
        ecs.update(deltaTime, scene);

        window.swapBuffers();
    }

    void loadScene(RenderAllocator &alloc) override {
        TextFileResource memStr("assets/sampleScene.json");
        scene = JsonSceneResource(memStr).getScene();
        res = ResourceFile("assets/sampleResources.json").getResources(alloc, domain);
        cameraNode = &scene.nodes.at("mainCamera");
    }

    void destroyScene() override {
        delete res;
    }

private:
    ColorRGBA clearColor = ColorRGBA(30, 30, 30, 255);

    MonoCppDomain domain;
    MonoCppAssembly *manaAssembly;

    ECS ecs;
    Scene scene;
    Resources *res;

    Node *cameraNode;
};

#endif //MANA_SAMPLE0_HPP
