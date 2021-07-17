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
    void start(Window &window, RenderDevice &device, Input &input) override {
        manaAssembly = domain.loadAssembly("mana.dll");

        ecs.addSystem(new ScriptingSystem(*res, input, domain, *manaAssembly));
        ecs.addSystem(new RenderSystem(window.getRenderTarget(), device));

        Game::start(window, device, input);
    }

    void stop(Window &window, RenderDevice &device, Input &input) override {
        delete manaAssembly;
        Game::stop(window, device, input);
    }

    void update(float deltaTime, Window &window, RenderDevice &device, Input &input) override {
        cameraNode->getComponent<CameraComponent>().camera.aspectRatio =
                (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        window.update();

        ecs.update(deltaTime, scene);

        window.swapBuffers();
    }

    void loadScene(RenderDevice &device) override {
        std::ifstream file;

        file.open("assets/resources.json", std::ifstream::in);
        if (file.fail())
            throw std::runtime_error("Failed to open assets/resources.json");
        res = ResourceManagerDeserializer(device, domain).deserialize(file);
        file.close();

        file.open("assets/scene.json");
        if (file.fail())
            throw std::runtime_error("Failed to open assets/scene.json");
        scene = SceneDeserializer(*res).deserialize(file);
        file.close();

        cameraNode = &scene.nodes.at("mainCamera");
    }

    void destroyScene() override {
        scene = {};
        delete res;
    }

private:
    ECS ecs;

    MonoCppDomain domain;
    MonoCppAssembly *manaAssembly;

    Scene scene;
    ResourceManager* res;

    Node *cameraNode;
};

#endif //MANA_SAMPLE0_HPP
