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

#include "gui/debugwindow.hpp"

using namespace engine;

class SampleApplication : public Application, InputListener {
public:
    SampleApplication(int argc, char *argv[])
            : Application(argc,
                          argv,
                          std::make_unique<DirectoryArchive>(std::filesystem::current_path().string() + "/assets")) {
        window->setSwapInterval(0);
        window->update();
        window->getRenderDevice().getRenderer().renderClear(window->getRenderTarget(), bgColor);
        window->swapBuffers();

        ren2d = std::make_unique<Renderer2D>(window->getRenderDevice());
        drawLoadingScreen(0);
    }

    ~SampleApplication() override = default;

protected:
    void start() override {
        renderSystem = new RenderSystem(*window, *archive, {});

        renderSystem->getRenderer().addRenderPass(std::move(std::make_unique<ForwardPass>(window->getRenderDevice())));
        drawLoadingScreen(0.1);
        renderSystem->getRenderer().addRenderPass(std::move(std::make_unique<PrePass>(window->getRenderDevice())));
        drawLoadingScreen(0.2);
        renderSystem->getRenderer().addRenderPass(
                std::move(std::make_unique<PhongShadePass>(window->getRenderDevice())));
        drawLoadingScreen(0.3);
        renderSystem->getRenderer().addRenderPass(std::move(std::make_unique<SkyboxPass>(window->getRenderDevice())));
        drawLoadingScreen(0.4);
        renderSystem->getRenderer().addRenderPass(std::move(std::make_unique<DebugPass>(window->getRenderDevice())));
        drawLoadingScreen(0.5);
        renderSystem->getRenderer().addRenderPass(std::move(std::make_unique<ImGuiPass>(*window)));
        drawLoadingScreen(0.6);

        //Move is required because the ECS destructor deletes the system pointers.
        ecs = std::move(ECS(
                {
                        new PlayerInputSystem(window->getInput()),
                        renderSystem
                }
        ));
        ecs.start();

        drawLoadingScreen(0.7);

        std::vector<Compositor::Layer> layers = {
                {"Skybox",         SkyboxPass::COLOR,        "",             DEPTH_TEST_ALWAYS},
                {"PhongShading",   PhongShadePass::COMBINED, PrePass::DEPTH, DEPTH_TEST_ALWAYS},
                {"Forward",        ForwardPass::COLOR,       ForwardPass::DEPTH},
                {"Phong Ambient",  PhongShadePass::AMBIENT,  "",             DEPTH_TEST_ALWAYS},
                {"Phong Diffuse",  PhongShadePass::DIFFUSE,  "",             DEPTH_TEST_ALWAYS},
                {"Phong Specular", PhongShadePass::SPECULAR, "",             DEPTH_TEST_ALWAYS},
                {"Normal Vectors", DebugPass::NORMALS,       "",             DEPTH_TEST_ALWAYS},
                {"Wireframe",      DebugPass::WIREFRAME,     "",             DEPTH_TEST_ALWAYS},
                {"Lights",         DebugPass::LIGHTS,        "",             DEPTH_TEST_ALWAYS},
                {"Depth",          PrePass::DEPTH,           "",             DEPTH_TEST_ALWAYS},
                {"Position",       PrePass::POSITION,        "",             DEPTH_TEST_ALWAYS},
                {"Normal",         PrePass::NORMAL,          "",             DEPTH_TEST_ALWAYS},
                {"Diffuse",        PrePass::DIFFUSE,         "",             DEPTH_TEST_ALWAYS},
                {"Ambient",        PrePass::AMBIENT,         "",             DEPTH_TEST_ALWAYS},
                {"Specular",       PrePass::SPECULAR,        "",             DEPTH_TEST_ALWAYS},
                {"Shininess",      PrePass::SHININESS_ID,    "",             DEPTH_TEST_ALWAYS},
                {"ImGui",          ImGuiPass::COLOR,         "",             DEPTH_TEST_ALWAYS},
        };

        renderSystem->getRenderer().getCompositor().setLayers(layers);

        debugWindow.setLayers(layers);

        int maxSamples = window->getRenderDevice().getMaxSampleCount();
        debugWindow.setMaxSamples(maxSamples);
        debugWindow.setSamples(1);

        debugWindow.setLayerActive("Phong Ambient", false);
        debugWindow.setLayerActive("Phong Diffuse", false);
        debugWindow.setLayerActive("Phong Specular", false);
        debugWindow.setLayerActive("Normal Vectors", false);
        debugWindow.setLayerActive("Wireframe", false);
        debugWindow.setLayerActive("Lights", false);
        debugWindow.setLayerActive("Depth", false);
        debugWindow.setLayerActive("Position", false);
        debugWindow.setLayerActive("Normal", false);
        debugWindow.setLayerActive("Diffuse", false);
        debugWindow.setLayerActive("Ambient", false);
        debugWindow.setLayerActive("Specular", false);
        debugWindow.setLayerActive("Shininess", false);

        debugWindow.setLayerActive("ImGui", false);
        debugWindow.setLayerPinned("ImGui", true);

        renderSystem->getRenderer().getRenderPass<ImGuiPass>().setWidgets({debugWindow});

        auto &device = window->getRenderDevice();

        auto stream = archive->open("/scene.json");
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
                transform.transform.setPosition(transform.transform.getPosition()
                                                + Vec3f(static_cast<float>(x) * 20.0f, 0,
                                                        -(static_cast<float>(y) * 20.0f)));

                componentManager.update(ent, transform);

                componentManager.create<MeshRenderComponent>(ent, planeRender);
            }
        }

        window->getInput().addListener(*this);

        drawLoadingScreen(1);

        Application::start();
    }

    void stop() override {
        window->getInput().removeListener(*this);

        ecs.getEntityManager().clear();
        ecs.stop();
        ecs = ECS();//TODO: ECS Systems clear set

        debugWindow = {};

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

        debugWindow.setDrawCalls(drawCalls);

        renderSystem->getRenderer().getCompositor().setLayers(debugWindow.getSelectedLayers());
        renderSystem->getRenderer().getGeometryBuffer().setSamples(debugWindow.getSamples());

        wnd.setSwapInterval(debugWindow.getSwapInterval());

        wnd.getRenderDevice().getRenderer().debugDrawCallRecordStart();

        ecs.update(deltaTime);

        drawCalls = wnd.getRenderDevice().getRenderer().debugDrawCallRecordStop();

        window->swapBuffers();

        fpsLimit = debugWindow.getFpsLimit();
    }

private:
    void onKeyDown(keyboard::Key key) override {
        if (key == keyboard::KEY_F1) {
            debugWindow.setLayerActive("ImGui", !debugWindow.getLayerActive("ImGui"));
        }
    }

    void onKeyUp(keyboard::Key key) override {}

    void drawLoadingScreen(float progress) {
        if (progress > 1)
            progress = 1;
        else if (progress < 0)
            progress = 0;

        ren2d->renderBegin(window->getRenderTarget(), true, {}, window->getRenderTarget().getSize(), bgColor);
        ren2d->setProjection({{-1, -1},
                              {1,  1}});
        float xdim = 0.5;
        float ydim = 0.05;
        ren2d->draw(Rectf({-xdim / 2, -ydim / 2}, {xdim, ydim}),
                    ColorRGBA(71, 71, 71, 255),
                    true);
        float xpos = -xdim / 2 * progress;
        float xscaledim = xdim * progress;
        xpos -= (xdim - xscaledim) / 2;
        ren2d->draw(Rectf({xpos, -ydim / 2}, {xscaledim, ydim}),
                    ColorRGBA(119, 255, 74, 255),
                    true);
        ren2d->renderPresent();

        window->swapBuffers();
        window->update();
    }

private:
    Entity cameraEntity;
    double fpsAverage = 1;
    unsigned long drawCalls = 0;// The number of draw calls in the last update

    RenderSystem *renderSystem{};

    DebugWindow debugWindow;

    std::unique_ptr<Renderer2D> ren2d;

    ColorRGBA bgColor = {38, 38, 38, 255};
};

#endif //MANA_SAMPLEAPPLICATION_HPP
