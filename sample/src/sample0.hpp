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

#include <filesystem>
#include <memory>

using namespace engine::runtime;

class Sample0 : public Game {
public:
    ~Sample0() override {
        charMap.clear();
    }

protected:
    void start(Window &window, RenderDevice &device, Input &input) override {
        archive = DirectoryArchive(std::filesystem::current_path().c_str());
        assetImporter = std::make_unique<AssetImporter>(archive);

        FontRasterizer *r = FontRasterizer::instantiate(FontRasterizer::FreeType, device.getAllocator());

        auto *stream = archive.open("assets/fonts/roboto/Roboto-Regular.ttf");
        Font *font = r->createFont(*stream);
        charMap = std::move(r->getAscii(*font));

        delete font;
        delete stream;
        delete r;

        texture = device.getAllocator().createTextureBuffer({});
        texture->upload(assetImporter->getBundle("assets/images/smiley.png").getImage());

        ren2d = Renderer2D(device);

        auto *assemblyStream = archive.open("Newtonsoft.Json.dll");
        jsonAssembly = domain.loadAssembly(*assemblyStream);
        delete assemblyStream;

        assemblyStream = archive.open("mana.dll");
        manaAssembly = domain.loadAssembly(*assemblyStream);
        delete assemblyStream;

        ecs.addSystem(new MonoScriptingSystem(input, domain, *manaAssembly, archive));
        ecs.addSystem(new RenderSystem(window.getRenderTarget(), device, *assetImporter));

        Game::start(window, device, input);
    }

    void stop(Window &window, RenderDevice &device, Input &input) override {
        delete manaAssembly;
        delete jsonAssembly;
        delete texture;
        Game::stop(window, device, input);
    }

    void update(float deltaTime, Window &window, RenderDevice &device, Input &input) override {
        cameraNode->getComponent<CameraComponent>().camera.aspectRatio =
                (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        window.update();

        auto windowSize = window.getFramebufferSize();

        if (deltaTime > 0) {
            float fps = 1.0f / deltaTime;
            float alpha = 0.9;
            rollingAverage = alpha * rollingAverage + (1.0 - alpha) * fps;
        }

        ecs.update(deltaTime, scene);

        ren2d.renderBegin(window.getRenderTarget(), false);
        ren2d.draw(Rectf({150, 300}, {100, 100}), *texture);
        ren2d.draw(Vec2f(50, 150),
                   Vec2f(1, 1),
                   std::to_string(rollingAverage),
                   charMap,
                   {255, 255, 255, 255});
        ren2d.renderPresent();

        window.swapBuffers();
    }

    void loadScene(RenderDevice &device) override {
        auto *sceneStream = archive.open("assets/scene.json");

        scene << JsonProtocol().deserialize(*sceneStream);

        delete sceneStream;

        cameraNode = &scene.nodes.at("MainCamera");

        auto &plane = scene.nodes.at("Plane");

        for (int i = 0; i < 100; i++) {
            std::string name = "Plane_Copy" + std::to_string(i);
            scene.nodes[name] = plane;
            scene.nodes[name].getComponent<TransformComponent>().transform.position += Vec3f(i * 20, 0, 0);
        }
    }

    void destroyScene() override {
        scene = {};
    }

private:
    Scene scene;

    ECS ecs;

    Renderer2D ren2d;

    MonoCppDomain domain;
    MonoCppAssembly *manaAssembly;
    MonoCppAssembly *jsonAssembly;

    Node *cameraNode;

    DirectoryArchive archive;

    std::unique_ptr<AssetImporter> assetImporter;

    float rot = 0;

    float rollingAverage = 1;

    TextureBuffer *texture;

    std::map<char, Character> charMap;
};

#endif //MANA_SAMPLE0_HPP
