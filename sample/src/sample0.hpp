#ifndef MANA_SAMPLE0_HPP
#define MANA_SAMPLE0_HPP

#include "game.hpp"

#include "shadersource.hpp"

class Sample0 : public Game {
public:
    void onFramebufferResize(Vec2i size) override {
        camera.aspectRatio = (float) size.x / (float) size.y;
    }

protected:
    void update(float deltaTime, Window &window, RenderAPI &renderApi, Input &input) override {
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

        Vec3f lightPos = renderCommand.pointLights.at(0).transform.position;

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

        renderCommand.pointLights.at(0).transform.position = lightPos;
        sphere->transform.position = lightPos;

        sphere->transform.rotation += Vec3f(rotationSpeed, rotationSpeed / 2, rotationSpeed);

        skybox->transform.position = camera.transform.position;

        forward = view * Vec4f(0, 0, -1, 0);

        renderCommand.spotLights.at(0).direction = toVec3(forward);
        renderCommand.spotLights.at(0).transform.position = camera.transform.position;

        renderApi.render(renderCommand, window.getFrameBuffer(), clearColor, true, true, true, false);

        window.swapBuffers();
        window.update();
    }

    void loadScene(RenderAPI &renderApi) override {
        Vec3f lightPos = Vec3f(2, 1, 0);

        PointLight light;
        light.transform.position = lightPos;
        light.constant = 1.0f;
        light.linear = 1.0f;
        light.quadratic = 1.0f;
        light.ambient = Vec3f(0);
        renderCommand.pointLights.emplace_back(light);

        Vec3f lightPos0 = Vec3f(-4, 0.73, 0);
        SpotLight light0 = SpotLight();
        light0.transform.position = lightPos0;
        light0.cutOff = 15;
        light0.outerCutOff = 25;
        renderCommand.spotLights.emplace_back(light0);

        Mesh planeMesh = MeshLoader::load("./assets/plane.obj");
        Mesh curveCubeMesh = MeshLoader::load("./assets/curvecube.obj");
        Mesh sphereMesh = MeshLoader::load("./assets/icosphere.obj");
        Mesh cubeMesh = MeshLoader::load("./assets/cube.obj");

        ShaderProgram *skyboxShader = renderApi.compileShaderProgram(skyboxVertexShader, skyboxFragmentShader);
        objects.emplace_back(skyboxShader);

        skyboxShader->activate();
        skyboxShader->setInt("skybox", 0);

        ShaderProgram *shader = renderApi.compileShaderProgram(vertexShader, fragmentShader);
        objects.emplace_back(shader);

        shader->activate();
        shader->setInt("diffuse", 0);
        shader->setInt("specular", 1);

        ShaderProgram *lightShader = renderApi.compileShaderProgram(vertexShader, lightFragmentShader);
        objects.emplace_back(lightShader);

        auto colorMap = renderApi.allocateTexture(ImageLoader::load("./assets/colormap.png"),
                                                  TextureAttributes());

        auto texture = ImageLoader::load("./assets/deepbluespace-skybox_maintex.png");
        std::vector<ImageBuffer<ColorRGBA32>> textures;
        textures.push_back(texture.slice(Recti(Vec2i(0, 0), Vec2i(2048, 2048))));
        textures.push_back(texture.slice(Recti(Vec2i(2048 * 1, 0), Vec2i(2048, 2048))));
        textures.push_back(texture.slice(Recti(Vec2i(2048 * 2, 0), Vec2i(2048, 2048))));
        textures.push_back(texture.slice(Recti(Vec2i(2048 * 3, 0), Vec2i(2048, 2048))));
        textures.push_back(texture.slice(Recti(Vec2i(2048 * 4, 0), Vec2i(2048, 2048))));
        textures.push_back(texture.slice(Recti(Vec2i(2048 * 5, 0), Vec2i(2048, 2048))));

        auto skyboxTexture = renderApi.allocateTexture(textures, TextureAttributes());

        objects.emplace_back(colorMap);
        objects.emplace_back(skyboxTexture);

        MeshObject *cubePtr = renderApi.allocateMesh(cubeMesh);
        MeshObject *curveCubePtr = renderApi.allocateMesh(curveCubeMesh);
        MeshObject *planePtr = renderApi.allocateMesh(planeMesh);
        MeshObject *spherePtr = renderApi.allocateMeshInstanced(sphereMesh, {
                Transform({0, 0, 0}, {}, {1, 1, 1}),
                Transform({0, 1, 0}, {}, {1, 1, 1}),
                Transform({0, -1, 0}, {}, {1, 1, 1}),
                Transform({1, 0, 0}, {}, {1, 1, 1}),
                Transform({-1, 0, 0}, {}, {1, 1, 1}),
                Transform({0, 0, 1}, {}, {1, 1, 1}),
                Transform({0, 0, -1}, {}, {1, 1, 1})
        });

        objects.emplace_back(cubePtr);
        objects.emplace_back(curveCubePtr);
        objects.emplace_back(planePtr);
        objects.emplace_back(spherePtr);

        RenderUnit unit;
        unit.enableDepthTest = true;

        unit.meshObjects.emplace_back(curveCubePtr);

        unit.shader = shader;

        unit.textureObjects.emplace_back(colorMap);

        unit.transform.position = Vec3f(0, 1, 0);
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(1);

        renderCommand.units.emplace(renderCommand.units.end(), unit);

        unit = RenderUnit();
        unit.enableDepthTest = true;

        unit.meshObjects.emplace_back(spherePtr);

        unit.shader = shader;

        unit.textureObjects.emplace_back(colorMap);

        unit.transform.position = lightPos;
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(0.1f);

        renderCommand.units.emplace(renderCommand.units.end(), unit);

        unit = RenderUnit();
        unit.enableDepthTest = true;

        unit.meshObjects.emplace_back(planePtr);

        unit.shader = shader;

        unit.textureObjects.emplace_back(colorMap);

        unit.transform.position = Vec3f(0);
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(10.0f);

        renderCommand.units.emplace(renderCommand.units.end(), unit);

        unit = RenderUnit();
        unit.enableDepthTest = false;
        unit.meshObjects.emplace_back(cubePtr);

        unit.shader = skyboxShader;

        unit.textureObjects.emplace_back(skyboxTexture);

        renderCommand.units.emplace(renderCommand.units.begin(), unit);

        renderCommand.camera = &camera;

        camera.transform.position = Vec3f(0, 3, 3);
        camera.transform.rotation = Vec3f(1, 0, 0);

        skybox = &*(renderCommand.units.begin());
        sphere = &*(renderCommand.units.begin() + 2);
        cube = &*(renderCommand.units.begin() + 1);
    }

    void destroyScene() override {
        for(auto *p : objects)
            delete p;
    }

private:
    std::vector<RenderObject*> objects;

    float cameraRotationSpeed = 45.0f;
    float cameraMovementSpeed = 5.0f;

    bool incrementLight = false;

    float lightMovementSpeed = 1.0f;

    Mouse mouseLastFrame;

    ColorRGBA32 clearColor = ColorRGBA32(30, 30, 30, 255);

    PerspectiveCamera camera;

    RenderUnit *skybox;
    RenderUnit *sphere;
    RenderUnit *cube;
};

#endif //MANA_SAMPLE0_HPP
