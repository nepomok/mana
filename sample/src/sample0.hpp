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

class Sample0 : public Game {
public:
    ~Sample0() override {
        charMap.clear();
    }

protected:
    void start(Window &window, RenderDevice &device, Input &input) override {
        archive = ArchiveDirectory(std::filesystem::current_path().c_str());
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

        auto *assemblyStream = archive.open("mana.dll");
        manaAssembly = domain.loadAssembly(*assemblyStream);
        delete assemblyStream;

        ecs.addSystem(new ScriptingSystem(input, domain, *manaAssembly, archive));
        ecs.addSystem(new RenderSystem(window.getRenderTarget(), device, *assetImporter));

        Game::start(window, device, input);
    }

    void stop(Window &window, RenderDevice &device, Input &input) override {
        delete manaAssembly;
        delete texture;
        Game::stop(window, device, input);
    }

    void update(float deltaTime, Window &window, RenderDevice &device, Input &input) override {
        cameraNode->getComponent<CameraComponent>().camera.aspectRatio =
                (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        window.update();

        ecs.update(deltaTime, scene);

        rot += deltaTime * 180;
        if (rot >= 360)
            rot = 0;

        ren2d.renderBegin(window.getRenderTarget(), false);
        ren2d.draw(Rectf(Vec2f(100, 100), Vec2f(100, 100)), ColorRGBA(0, 125, 125, 255), false, {50, 50}, rot);
        ren2d.draw(Rectf(Vec2f(100, 300), Vec2f(100, 100)), ColorRGBA(0, 125, 125, 125), true, {50, 50}, -rot);
        ren2d.draw(Rectf(Vec2f(125, 325), Vec2f(50, 50)), ColorRGBA(255, 0, 0, 125), true, {25, 25}, rot);
        ren2d.draw(Rectf(Vec2f(100, 500), Vec2f(100, 100)), *texture, {50, 50}, rot);
        ren2d.draw(Rectf({0, 0}, {50, 50}), Rectf(Vec2f(100, 700), Vec2f(100, 100)), *texture, {50, 50}, -rot);
        ren2d.draw(window.getFramebufferSize().convert<float>(), Vec2f(), ColorRGBA(0, 255, 0, 125));
        ren2d.draw(Vec2f(0.5, 0.4), ColorRGBA(255, 255, 255, 255));
        ren2d.draw(Vec2f(0.3, 0.1), "Hello World !!", charMap, ColorRGBA(255, 255, 255, 125));

        //Normalized screen coordinates through setting the projection bounds
        ren2d.setProjection(Rectf({}, {1, 1}));
        ren2d.draw(Vec2f(1, 0), Vec2f(0, 1), ColorRGBA(255, 0, 0, 125));

        ren2d.renderPresent();

        window.swapBuffers();
    }

    void loadScene(RenderDevice &device) override {
        auto *sceneStream = archive.open("assets/scene.json");
        scene = SceneDeserializer().deserialize(*sceneStream);
        delete sceneStream;

        cameraNode = &scene.nodes.at("MainCamera");
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

    Node *cameraNode;

    ArchiveDirectory archive;

    std::unique_ptr<AssetImporter> assetImporter;

    float rot = 0;

    TextureBuffer *texture;

    std::map<char, Character> charMap;
};

#endif //MANA_SAMPLE0_HPP
