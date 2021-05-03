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

#ifndef MANA_SAMPLE2_HPP
#define MANA_SAMPLE2_HPP


#include "game.hpp"

class Sample2 : public Game {
public:
    ~Sample2() override = default;

protected:
    void start(Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        Game::start(window, ren, alloc, input);
        ecs.addSystem(new RenderSystem(window.getRenderTarget(), ren3d));
        ecs.addSystem(new ScriptingSystem(monoRuntime, monoRuntime.getMsCorLibAssembly(), *manaAssembly));
        ren.setMultiSample(true);
    }

    void stop(Window &window, Renderer &renderApi, RenderAllocator &alloc, Input &input) override {
        delete manaAssembly;
        Game::stop(window, renderApi, alloc, input);
    }

    void update(float deltaTime, Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        MonoCppValue v;
        v.setValue<float>(&deltaTime);
        manaAssembly->setStaticField("Mana", "Time", "deltaTime", v);

        Mouse mouse = input.getMouse();
        Vec2d mouseDiff = mouse.position - mouseLastFrame.position;
        mouseLastFrame = mouse;

        double rotationSpeed = cameraRotationSpeed * deltaTime;

        if (input.getKeyDown(KEY_UP)) {
            cameraNode->getComponent<TransformComponent>().transform.rotation.x += rotationSpeed;
        } else if (input.getKeyDown(KEY_DOWN)) {
            cameraNode->getComponent<TransformComponent>().transform.rotation.x -= rotationSpeed;
        }

        if (input.getKeyDown(KEY_LEFT)) {
            cameraNode->getComponent<TransformComponent>().transform.rotation.y -= rotationSpeed;
        } else if (input.getKeyDown(KEY_RIGHT)) {
            cameraNode->getComponent<TransformComponent>().transform.rotation.y += rotationSpeed;
        }

        Vec3f inputMovement = Vec3f(0);
        if (input.getKeyDown(KEY_A)) {
            inputMovement.x = -1;
        } else if (input.getKeyDown(KEY_D)) {
            inputMovement.x = 1;
        }

        if (input.getKeyDown(KEY_W)) {
            inputMovement.z = -1;
        } else if (input.getKeyDown(KEY_S)) {
            inputMovement.z = 1;
        }

        if (input.getKeyDown(KEY_E)) {
            inputMovement.y = 1;
        } else if (input.getKeyDown(KEY_Q)) {
            inputMovement.y = -1;
        }

        cameraNode->getComponent<CameraComponent>().aspectRatio =
                (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        PerspectiveCamera cam;
        cam.transform = cameraNode->getComponent<TransformComponent>().transform;
        cam.nearClip = cameraNode->getComponent<CameraComponent>().nearClip;
        cam.farClip = cameraNode->getComponent<CameraComponent>().farClip;
        cam.fov = cameraNode->getComponent<CameraComponent>().fov;
        cam.aspectRatio = cameraNode->getComponent<CameraComponent>().aspectRatio;

        //Translate direction vectors with desired length into world space and add them to the current camera position.
        Mat4f view = cam.view();

        view = MatrixMath::inverse(view);

        float movementSpeed = cameraMovementSpeed * deltaTime;

        Vec4f left = (view) * Vec4f(inputMovement.x * movementSpeed, 0, 0, 0);
        Vec4f up = (view) * Vec4f(0, inputMovement.y * movementSpeed, 0, 0);
        Vec4f forward = (view) * Vec4f(0, 0, inputMovement.z * movementSpeed, 0);

        cameraNode->getComponent<TransformComponent>().transform.position += toVec3(left + up + forward);

        ren.setViewport({}, window.getFramebufferSize());

        ecs.update(deltaTime, scene);

        window.swapBuffers();
        window.update();
    }

    void loadScene(RenderAllocator &alloc) override {
        //IMPORTANT: All assemblies referenced in other assemblies have to be loaded first.
        manaAssembly = monoRuntime.loadAssembly("assets/mana.dll");
        scene = SceneFile("assets/sampleScene.json").loadScene(alloc, monoRuntime);
        cameraNode = &scene.nodes.at("mainCamera");
    }

    void destroyScene() override {
        for (auto *p : objects)
            delete p;
    }

private:
    std::vector<RenderObject *> objects;

    float cameraRotationSpeed = 45.0f;
    float cameraMovementSpeed = 5.0f;

    Mouse mouseLastFrame;

    ColorRGBA clearColor = ColorRGBA(30, 30, 30, 255);

    ECS ecs;

    Scene scene;

    Node *cameraNode;

    MonoCppRuntime monoRuntime;

    MonoCppAssembly *manaAssembly;
};

#endif //MANA_SAMPLE2_HPP
