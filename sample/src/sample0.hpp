#ifndef MANA_SAMPLE0_HPP
#define MANA_SAMPLE0_HPP

#include "game.hpp"

#include "shadersource.hpp"

class Sample0 : public Game {
public:
    void onFramebufferResize(Vec2i size) override {
        scene.camera.cameraData.perspective.aspectRatio = (float) size.x / (float) size.y;
    }

protected:
    void update(float deltaTime, Window &window, RenderAPI &renderApi, Input &input) override {
        Mouse mouse = input.getMouse();
        Vec2d mouseDiff = mouse.position - mouseLastFrame.position;
        mouseLastFrame = mouse;

        double rotationSpeed = cameraRotationSpeed * deltaTime;

        if (mouse.leftButtonDown && (mouseDiff.x != 0 || mouseDiff.y != 0)) {
            scene.units.at(1).transform.rotation.x += mouseDiff.y * rotationSpeed;
            scene.units.at(1).transform.rotation.y += mouseDiff.x * rotationSpeed;
        }

        if (input.getKeyDown(KEY_UP)) {
            scene.camera.transform.rotation.x += rotationSpeed;
        } else if (input.getKeyDown(KEY_DOWN)) {
            scene.camera.transform.rotation.x -= rotationSpeed;
        }

        if (input.getKeyDown(KEY_LEFT)) {
            scene.camera.transform.rotation.y -= rotationSpeed;
        } else if (input.getKeyDown(KEY_RIGHT)) {
            scene.camera.transform.rotation.y += rotationSpeed;
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
        Mat4f view = MatrixMath::identity();
        view = view * MatrixMath::translate(scene.camera.transform.position);
        view = view * MatrixMath::rotate(Vec3f(scene.camera.transform.rotation.x,
                                               scene.camera.transform.rotation.y,
                                               scene.camera.transform.rotation.z));

        view = MatrixMath::inverse(view);

        float movementSpeed = cameraMovementSpeed * deltaTime;

        Vec4f left = (view) * Vec4f(inputMovement.x * movementSpeed, 0, 0, 0);
        Vec4f up = (view) * Vec4f(0, inputMovement.y * movementSpeed, 0, 0);
        Vec4f forward = (view) * Vec4f(0, 0, inputMovement.z * movementSpeed, 0);

        scene.camera.transform.position += (Vec3f(forward.x, forward.y, forward.z))
                                           + (Vec3f(left.x, left.y, left.z))
                                           + (Vec3f(up.x, up.y, up.z));

        Vec3f lightPos = scene.pointLights.at(0).transform.position;

        Mat4f rot = MatrixMath::rotate(Vec3f(0, rotationSpeed, 0));
        Vec4f pos4 = rot * Vec4f(lightPos.x, lightPos.y, lightPos.z, 1);

        lightPos = Vec3f(pos4.x, pos4.y, pos4.z);

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

        scene.pointLights.at(0).transform.position = lightPos;
        scene.units.at(2).transform.position = lightPos;

        scene.units.at(2).transform.rotation += Vec3f(rotationSpeed, rotationSpeed / 2, rotationSpeed);

        scene.units.at(0).transform.position = scene.camera.transform.position;

        forward = view * Vec4f(0, 0, -1, 0);

        scene.spotLights.at(0).direction = Vec3f(forward.x, forward.y, forward.z);
        scene.spotLights.at(0).transform.position = scene.camera.transform.position;
        renderApi.render(scene, window.getFrameBuffer(), clearColor, true, true, true, false);

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
        scene.pointLights.emplace_back(light);

        Vec3f lightPos0 = Vec3f(-4, 0.73, 0);
        SpotLight light0 = SpotLight();
        light0.transform.position = lightPos0;
        light0.cutOff = 15;
        light0.outerCutOff = 25;
        scene.spotLights.emplace_back(light0);

        RenderMesh planeMesh = MeshLoader::load("./assets/plane.obj");
        RenderMesh curveCubeMesh = MeshLoader::load("./assets/curvecube.obj");
        RenderMesh sphereMesh = MeshLoader::load("./assets/icosphere.obj");
        RenderMesh cubeMesh = MeshLoader::load("./assets/cube.obj");

        ShaderProgram *skyboxShader = renderApi.compileShaderProgram(skyboxVertexShader, skyboxFragmentShader);
        res.shaders.emplace_back(skyboxShader);
        skyboxShader->activate();
        skyboxShader->setInt("skybox", 0);

        ShaderProgram *shader = renderApi.compileShaderProgram(vertexShader, fragmentShader);
        res.shaders.emplace_back(shader);

        shader->activate();
        shader->setInt("diffuse", 0);
        shader->setInt("specular", 1);

        ShaderProgram *lightShader = renderApi.compileShaderProgram(vertexShader, lightFragmentShader);
        res.shaders.emplace_back(lightShader);

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

        res.textures.emplace_back(colorMap);
        res.textures.emplace_back(skyboxTexture);

        MeshObject *cubePtr = renderApi.allocateMesh(cubeMesh);
        MeshObject *curveCubePtr = renderApi.allocateMesh(curveCubeMesh);
        MeshObject *planePtr = renderApi.allocateMesh(planeMesh);
        MeshObject *spherePtr = renderApi.allocateMesh(sphereMesh);

        res.meshes.emplace_back(cubePtr);
        res.meshes.emplace_back(curveCubePtr);
        res.meshes.emplace_back(planePtr);
        res.meshes.emplace_back(spherePtr);

        RenderUnit unit;
        unit.enableDepthTest = true;

        unit.meshData.emplace_back(curveCubePtr);

        unit.shader = shader;

        unit.textures.emplace_back(colorMap);

        unit.transform.position = Vec3f(0, 1, 0);
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(1);

        scene.units.emplace_back(unit);

        unit = RenderUnit();
        unit.enableDepthTest = true;

        unit.meshData.emplace_back(spherePtr);

        unit.shader = shader;

        unit.textures.emplace_back(colorMap);

        unit.transform.position = lightPos;
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(0.1f);

        scene.units.emplace_back(unit);

        unit = RenderUnit();
        unit.enableDepthTest = true;

        unit.meshData.emplace_back(planePtr);

        unit.shader = shader;

        unit.textures.emplace_back(colorMap);

        unit.transform.position = Vec3f(0);
        unit.transform.rotation = Vec3f(0);
        unit.transform.scale = Vec3f(10.0f);

        scene.units.emplace_back(unit);

        unit = RenderUnit();
        unit.enableDepthTest = false;
        unit.meshData.emplace_back(cubePtr);

        unit.shader = skyboxShader;

        unit.textures.emplace_back(skyboxTexture);

        scene.units.emplace(scene.units.begin(), unit);

        scene.camera.transform.position = Vec3f(0, 3, 3);
        scene.camera.transform.rotation = Vec3f(1, 0, 0);
    }

    void destroyScene() override {
        for (auto *p : res.shaders)
            delete p;
        for (auto *p : res.textures)
            delete p;
        for (auto *p : res.meshes)
            delete p;
    }

private:
    struct Resources {
        std::vector<ShaderProgram *> shaders;
        std::vector<TextureObject *> textures;
        std::vector<MeshObject *> meshes;
    } res;

    float cameraRotationSpeed = 30.0f;
    float cameraMovementSpeed = 5.0f;

    bool incrementLight = false;

    float lightMovementSpeed = 1.0f;

    Mouse mouseLastFrame;
    ColorRGBA32 clearColor = ColorRGBA32(30, 30, 30, 255);
};

#endif //MANA_SAMPLE0_HPP
