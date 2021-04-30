/**
 *  Mana - 3D Engine
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

#include "shadersource.hpp"

class Sample0 : public Game {
public:
    ~Sample0() override = default;

protected:
    void start(Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        Game::start(window, ren, alloc, input);
    }

    void update(float deltaTime, Window &window, Renderer &ren, RenderAllocator &alloc, Input &input) override {
        Mouse mouse = input.getMouse();
        Vec2d mouseDiff = mouse.position - mouseLastFrame.position;
        mouseLastFrame = mouse;

        double rotationSpeed = cameraRotationSpeed * deltaTime;

        if (mouse.leftButtonDown && (mouseDiff.x != 0 || mouseDiff.y != 0)) {
            cube->transform.rotation.x += mouseDiff.y * rotationSpeed;
            cube->transform.rotation.y += mouseDiff.x * rotationSpeed;
        }

        if (input.getKeyDown(KEY_UP)) {
            camera.transform.rotation.x += rotationSpeed;
        } else if (input.getKeyDown(KEY_DOWN)) {
            camera.transform.rotation.x -= rotationSpeed;
        }

        if (input.getKeyDown(KEY_LEFT)) {
            camera.transform.rotation.y -= rotationSpeed;
        } else if (input.getKeyDown(KEY_RIGHT)) {
            camera.transform.rotation.y += rotationSpeed;
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

        //Translate direction vectors with desired length into world space and add them to the current camera position.
        Mat4f view = camera.view();

        view = MatrixMath::inverse(view);

        float movementSpeed = cameraMovementSpeed * deltaTime;

        Vec4f left = (view) * Vec4f(inputMovement.x * movementSpeed, 0, 0, 0);
        Vec4f up = (view) * Vec4f(0, inputMovement.y * movementSpeed, 0, 0);
        Vec4f forward = (view) * Vec4f(0, 0, inputMovement.z * movementSpeed, 0);

        camera.transform.position += toVec3(left + up + forward);

        Vec3f lightPos = lights.point.at(0).transform.position;

        Mat4f rot = MatrixMath::rotate(Vec3f(0, rotationSpeed, 0));

        lightPos = toVec3(rot * toVec4(lightPos, 1));

        if (incrementLight) {
            if (lightPos.y < 2) {
                lightPos.y += lightMovementSpeed * deltaTime;
            } else {
                lightPos.y = 2;
                incrementLight = false;
            }
        } else {
            if (lightPos.y > 0.5f) {
                lightPos.y -= lightMovementSpeed * deltaTime;
            } else {
                lightPos.y = 0.5f;
                incrementLight = true;
            }
        }

        lights.point.at(0).transform.position = lightPos;
        sphere->transform.position = lightPos;

        sphere->transform.rotation += Vec3f(rotationSpeed, rotationSpeed / 2, rotationSpeed);

        skybox->transform.position = camera.transform.position;

        forward = view * Vec4f(0, 0, -1, 0);

        lights.spot.at(0).direction = toVec3(forward);
        lights.spot.at(0).transform.position = camera.transform.position;

        camera.aspectRatio = (float) window.getFramebufferSize().x / (float) window.getFramebufferSize().y;

        ren.setViewport({}, window.getFramebufferSize());

        ren3d.render(window.getRenderTarget(), camera, commands, lights);

        window.swapBuffers();
        window.update();
    }

    void loadScene(RenderAllocator &alloc) override {
        Vec3f lightPos = Vec3f(2, 1, 0);

        PointLight light;
        light.transform.position = lightPos;
        light.constant = 1.0f;
        light.linear = 1.0f;
        light.quadratic = 1.0f;
        light.ambient = Vec3f(0);
        lights.point.emplace_back(light);

        Vec3f lightPos0 = Vec3f(-4, 0.73, 0);
        SpotLight light0 = SpotLight();
        light0.transform.position = lightPos0;
        light0.cutOff = 15;
        light0.outerCutOff = 25;
        lights.spot.emplace_back(light0);

        Mesh planeMesh = MeshLoader::load("./assets/plane.obj");
        Mesh curveCubeMesh = MeshLoader::load("./assets/curvecube.obj");
        Mesh sphereMesh = MeshLoader::load("./assets/icosphere.obj");
        Mesh cubeMesh = MeshLoader::load("./assets/cube.obj");

        ShaderProgram *skyboxShader = alloc.allocateShaderProgram(Renderer3D::preprocessGlsl(skyboxVertexShader),
                                                                  Renderer3D::preprocessGlsl(skyboxFragmentShader));
        objects.emplace_back(skyboxShader);

        skyboxShader->setInt("skybox", 0);

        shader = alloc.allocateShaderProgram(Renderer3D::preprocessGlsl(vertexShader),
                                             Renderer3D::preprocessGlsl(fragmentShader));
        objects.emplace_back(shader);

        shader->setInt("diffuse", 0);
        shader->setInt("specular", 1);

        ShaderProgram *lightShader = alloc.allocateShaderProgram(Renderer3D::preprocessGlsl(vertexShader),
                                                                 Renderer3D::preprocessGlsl(lightFragmentShader));
        objects.emplace_back(lightShader);

        auto colorMapImage = ImageLoader::load("./assets/colormap.png");

        RenderTexture::Attributes attributes;
        attributes.size = colorMapImage.getSize();

        auto colorMapTexture = alloc.allocateTexture(attributes);

        colorMapTexture->upload(colorMapImage);

        auto skyboxImage = ImageLoader::load("./assets/deepbluespace-skybox_maintex.png");

        attributes = {};
        attributes.size = {2048, 2048};
        attributes.textureType = mana::RenderTexture::TEXTURE_CUBE_MAP;

        auto skyboxTexture = alloc.allocateTexture(attributes);

        skyboxTexture->upload(RenderTexture::RIGHT,
                              skyboxImage.slice(Recti(Vec2i(0, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::LEFT,
                              skyboxImage.slice(Recti(Vec2i(2048 * 1, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::TOP,
                              skyboxImage.slice(Recti(Vec2i(2048 * 2, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::BOTTOM,
                              skyboxImage.slice(Recti(Vec2i(2048 * 3, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::FRONT,
                              skyboxImage.slice(Recti(Vec2i(2048 * 4, 0), Vec2i(2048, 2048))));

        skyboxTexture->upload(RenderTexture::BACK,
                              skyboxImage.slice(Recti(Vec2i(2048 * 5, 0), Vec2i(2048, 2048))));

        objects.emplace_back(colorMapTexture);
        objects.emplace_back(skyboxTexture);

        RenderMesh *cubePtr = alloc.allocateMesh(cubeMesh);
        RenderMesh *curveCubePtr = alloc.allocateMesh(curveCubeMesh);
        RenderMesh *planePtr = alloc.allocateMesh(planeMesh);
        RenderMesh *spherePtr = alloc.allocateInstancedMesh(sphereMesh, {Transform({0, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, 1, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, -1, 0}, {}, {1, 1, 1}),
                                                                         Transform({1, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({-1, 0, 0}, {}, {1, 1, 1}),
                                                                         Transform({0, 0, 1}, {}, {1, 1, 1}),
                                                                         Transform({0, 0, -1}, {}, {1, 1, 1})});

        objects.emplace_back(cubePtr);
        objects.emplace_back(curveCubePtr);
        objects.emplace_back(planePtr);
        objects.emplace_back(spherePtr);

        RenderCommand command;
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(curveCubePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = Vec3f(0, 1, 0);
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(1);

        commands.emplace(commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(spherePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = lightPos;
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(0.1f);

        commands.emplace(commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = true;

        command.meshObjects.emplace_back(planePtr);

        command.shader = shader;

        command.textureObjects.emplace_back(colorMapTexture);

        command.transform.position = Vec3f(0);
        command.transform.rotation = Vec3f(0);
        command.transform.scale = Vec3f(10.0f);

        commands.emplace(commands.end(), command);

        command = RenderCommand();
        command.enableDepthTest = false;
        command.meshObjects.emplace_back(cubePtr);

        command.shader = skyboxShader;

        command.textureObjects.emplace_back(skyboxTexture);

        commands.emplace(commands.begin(), command);

        camera.transform.position = Vec3f(0, 3, 3);
        camera.transform.rotation = Vec3f(1, 0, 0);

        skybox = &*(commands.begin());
        sphere = &*(commands.begin() + 2);
        cube = &*(commands.begin() + 1);
    }

    void destroyScene() override {
        for (auto *p : objects)
            delete p;
    }

private:
    std::vector<RenderObject *> objects;

    float cameraRotationSpeed = 45.0f;
    float cameraMovementSpeed = 5.0f;

    bool incrementLight = false;

    float lightMovementSpeed = 1.0f;

    Mouse mouseLastFrame;

    ColorRGBA clearColor = ColorRGBA(30, 30, 30, 255);

    PerspectiveCamera camera;

    LightingData lights;

    std::vector<RenderCommand> commands;

    RenderCommand *skybox;
    RenderCommand *sphere;
    RenderCommand *cube;

    ShaderProgram *shader;
};

#endif //MANA_SAMPLE0_HPP
