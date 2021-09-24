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

#ifndef MANA_SAMPLEAPPLICATION_HPP
#define MANA_SAMPLEAPPLICATION_HPP

#include "engine.hpp"

#include <filesystem>
#include <memory>

#include "systems/playerinputsystem.hpp"
#include "components/playercontrollercomponent.hpp"

using namespace engine;

class SampleApplication : public Application {
public:
    SampleApplication(int argc, char *argv[])
            : Application(argc, argv, new DirectoryArchive(std::filesystem::current_path().string())) {
    }

    ~SampleApplication() override = default;

protected:
    void start() override {
        archive = new DirectoryArchive(std::filesystem::current_path().c_str());

        auto &device = window->getRenderDevice();

        auto *stream = archive->open("assets/fonts/roboto/Roboto-Regular.ttf");
        Font *font = Font::createFont(*stream);
        font->setPixelSize({0, 30});
        characters = std::move(font->renderAscii());
        delete font;
        delete stream;

        for (auto &p : characters) {
            textures[p.first] = device.getAllocator().createTextureBuffer({})->upload(p.second.image);
        }

        texture = device.getAllocator().createTextureBuffer({});
        texture->upload(AssetImporter::import("assets/images/smiley.png", *archive).getImage());

        ren2d = Renderer2D(device);

        auto *assemblyStream = archive->open("Newtonsoft.Json.dll");
        jsonAssembly = domain.loadAssembly(*assemblyStream);
        delete assemblyStream;

        assemblyStream = archive->open("mana.dll");
        manaAssembly = domain.loadAssembly(*assemblyStream);
        delete assemblyStream;

        ecs.addSystem(new MonoScriptingSystem(window->getInput(), domain, *manaAssembly, *archive));
        ecs.addSystem(new PlayerInputSystem(window->getInput()));
        ecs.addSystem(new RenderSystem(window->getRenderTarget(), device, *archive));

        auto *sceneStream = archive->open("assets/scene.json");
        ecs.getEntityManager() << JsonProtocol().deserialize(*sceneStream);
        delete sceneStream;

        auto &entityManager = ecs.getEntityManager();
        auto &componentManager = entityManager.getComponentManager();

        cameraEntity = entityManager.getByName("MainCamera");

        componentManager.create<PlayerControllerComponent>(cameraEntity);

        auto planeEntity = entityManager.getByName("Plane");
        auto planeTransform = componentManager.lookup<TransformComponent>(planeEntity);
        auto planeRender = componentManager.lookup<MeshRenderComponent>(planeEntity);
        for (int x = 0; x < 10; x++) {
            for (int y = 0; y < 10; y++) {
                auto ent = entityManager.create();
                auto &transform = componentManager.create<TransformComponent>(ent);
                transform = planeTransform;
                transform.transform.position += Vec3f(x * 20, 0, -(y * 20));

                auto &render = componentManager.create<MeshRenderComponent>(ent);
                render = planeRender;
            }
        }

        Application::start();
    }

    void stop() override {
        ecs.getEntityManager().clear();

        delete manaAssembly;
        delete jsonAssembly;
        delete texture;

        for (auto &p : textures)
            delete p.second;

        characters.clear();
        textures.clear();

        Application::stop();
    }

    void update(float deltaTime) override {
        auto &wnd = *window;

        wnd.update();

        auto wndSize = wnd.getFramebufferSize();

        auto &entityManager = ecs.getEntityManager();
        auto &componentManager = entityManager.getComponentManager();

        componentManager.lookup<CameraComponent>(cameraEntity).camera.aspectRatio =
                (float) wndSize.x / (float) wndSize.y;

        if (deltaTime > 0) {
            float fps = 1.0f / deltaTime;
            float alpha = 0.9;
            fpsAverage = alpha * fpsAverage + (1.0 - alpha) * fps;
        }

        ecs.update(deltaTime);

        ren2d.renderBegin(wnd.getRenderTarget(), false);
        ren2d.draw(Rectf({150, 300}, {100, 100}), *texture);

        auto str = std::to_string(fpsAverage);
        ren2d.draw(Vec2f(10, 10 + Character::getMetrics(str, characters).position.y),
                   str,
                   {255, 255, 255, 255},
                   characters,
                   textures);

        ren2d.renderPresent();

        window->swapBuffers();
    }

private:
    Renderer2D ren2d;

    MonoCppDomain domain;
    MonoCppAssembly *manaAssembly = nullptr;
    MonoCppAssembly *jsonAssembly = nullptr;

    Entity cameraEntity;
    TextureBuffer *texture = nullptr;
    double fpsAverage = 1;

    std::map<char, Character> characters;
    std::map<char, TextureBuffer *> textures;
};

#endif //MANA_SAMPLEAPPLICATION_HPP
