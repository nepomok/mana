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

#include "gui/layerselection.hpp"

using namespace engine;

class SampleApplication : public Application, InputListener {
public:
    SampleApplication(int argc, char *argv[])
            : Application(argc,
                          argv,
                          std::make_unique<DirectoryArchive>(std::filesystem::current_path().string() + "/assets")) {}

    ~SampleApplication() override = default;

protected:
    void start() override {
        window->setSwapInterval(1);

        renderSystem = new RenderSystem(window->getRenderTarget(), window->getRenderDevice(), *archive);

        //Move is required because the ECS destructor deletes the system pointers.
        ecs = std::move(ECS(
                {
                        new PlayerInputSystem(window->getInput()),
                        renderSystem
                }
        ));
        ecs.start();

        auto &device = window->getRenderDevice();

        auto stream = archive->open("/fonts/roboto/Roboto-Regular.ttf");
        auto font = Font::createFont(*stream);
        font->setPixelSize({0, 30});
        characters = font->renderAscii();

        for (auto &p: characters) {
            textures[p.first] = device.getAllocator().createTextureBuffer({});
            textures[p.first]->upload(p.second.image);
        }

        texture = device.getAllocator().createTextureBuffer({});
        texture->upload(AssetImporter::import("/images/smiley_2.png", *archive).getImage());

        ren2d = std::make_unique<Renderer2D>(device);

        stream = archive->open("/scene.json");
        ecs.getEntityManager() << JsonProtocol().deserialize(*stream);

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
                auto transform = componentManager.create<TransformComponent>(ent);
                transform = planeTransform;
                transform.transform.position += Vec3f(static_cast<float>(x) * 20.0f, 0,
                                                      -(static_cast<float>(y) * 20.0f));

                componentManager.update(ent, transform);

                componentManager.create<MeshRenderComponent>(ent, planeRender);
            }
        }

        window->getInput().addListener(*this);

        layerSelection = std::make_unique<LayerSelection>(*window, displayBackend, graphicsBackend);

        Application::start();
    }

    void stop() override {
        window->getInput().removeListener(*this);

        ecs.getEntityManager().clear();
        ecs.stop();
        ecs = ECS();//TODO: ECS Systems clear set

        characters.clear();
        textures.clear();

        layerSelection = {};

        Application::stop();
    }

    void update(float deltaTime) override {
        auto &wnd = *window;

        wnd.update();

        auto wndSize = wnd.getFramebufferSize();

        auto &entityManager = ecs.getEntityManager();
        auto &componentManager = entityManager.getComponentManager();

        auto cam = componentManager.lookup<CameraComponent>(cameraEntity);
        cam.camera.aspectRatio = (float) wndSize.x / (float) wndSize.y;
        componentManager.update<CameraComponent>(cameraEntity, cam);

        if (deltaTime > 0) {
            float fps = 1.0f / deltaTime;
            float alpha = 0.9;
            fpsAverage = alpha * fpsAverage + (1.0 - alpha) * fps;
        }

        ecs.update(deltaTime);

        ren2d->renderBegin(wnd.getRenderTarget(), false);
        ren2d->draw(Rectf({150, 300}, {100, 100}), *texture);

        auto str = std::to_string(fpsAverage);
        ren2d->draw(Vec2f(10, 10 + Character::getMetrics(str, characters).position.y),
                    str,
                    {255, 255, 255, 255},
                    characters,
                    textures);

        ren2d->renderPresent();

        layerSelection->render(wnd.getRenderTarget());

        window->swapBuffers();
    }

private:
    void onKeyDown(keyboard::Key key) override {
        if (key == keyboard::KEY_F1) {
            f1Toggle = !f1Toggle;
            renderSystem->setDrawDebugNormals(f1Toggle);
        } else if (key == keyboard::KEY_F2) {
            f2Toggle = !f2Toggle;
            renderSystem->setDrawDebugLightCasters(f2Toggle);
        }
    }

    void onKeyUp(keyboard::Key key) override {}

private:
    std::unique_ptr<Renderer2D> ren2d;

    Entity cameraEntity;
    std::unique_ptr<TextureBuffer> texture = nullptr;
    double fpsAverage = 1;

    std::map<char, Character> characters;
    std::map<char, std::unique_ptr<TextureBuffer>> textures;

    RenderSystem *renderSystem{};

    bool f1Toggle = false;
    bool f2Toggle = false;

    std::unique_ptr<LayerSelection> layerSelection;
};

#endif //MANA_SAMPLEAPPLICATION_HPP
