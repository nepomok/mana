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

#include "engine/render/2d/renderer2d.hpp"
#include "engine/math/matrixmath.hpp"

#include "engine/asset/camera.hpp"

static const char *SHADER_GLOBALS = R"###(
float4x4 MODEL_MATRIX;
float USE_TEXTURE;
float4 COLOR;
)###";

static const char *SHADER_VERT = R"###(
#include "globals.hlsl"

struct VS_INPUT
{
    float3 position : POSITION0;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 instanceRow0 : POSITION1;
    float4 instanceRow1 : POSITION2;
    float4 instanceRow2 : POSITION3;
    float4 instanceRow3 : POSITION4;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2  uv : TEXCOORD0;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;

    ret.position = mul(float4(v.position, 1), MODEL_MATRIX);
    ret.uv = v.uv;

    return ret;
}
)###";

static const char *SHADER_FRAG = R"###(
#include "globals.hlsl"

struct PS_INPUT {
    float2 uv: TEXCOORD0;
};

struct PS_OUTPUT {
     float4 pixel     :   SV_TARGET0;
};

Texture2D diffuse;

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    if (USE_TEXTURE != 0)
        ret.pixel = diffuse.Sample(samplerState_diffuse, v.uv);
    else
        ret.pixel = COLOR;
    return ret;
}
)###";

static const char *SHADER_TEXT_FRAG = R"###(
#include "globals.hlsl"

struct PS_INPUT {
    float2 uv: TEXCOORD0;
};

struct PS_OUTPUT {
     float4 pixel     :   SV_TARGET0;
};

Texture2D diffuse;

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    float grayscale = diffuse.Sample(samplerState_diffuse, v.uv).r;
    ret.pixel = COLOR * grayscale;
    return ret;
}
)###";

static std::string includeCallback(const char *name) {
    return SHADER_GLOBALS;
}

static float distance(float val1, float val2) {
    float abs = val1 - val2;
    if (abs < 0)
        return abs * -1;
    else
        return abs;
}

namespace mana {
    /**
     * Get plane mesh with origin at top left offset by center and scaled in the y axis.
     *
     * @param size
     * @return
     */
    static Mesh getPlane(Vec2f size, Vec2f center, Rectf uvOffset) {
        Rectf scaledOffset(
                {uvOffset.position.x / size.x, uvOffset.position.y / size.y},
                {uvOffset.dimensions.x / size.x, uvOffset.dimensions.y / size.y});
        float uvNearX = scaledOffset.position.x;
        float uvFarX = scaledOffset.position.x + scaledOffset.dimensions.x;
        float uvNearY = scaledOffset.position.y;
        float uvFarY = scaledOffset.position.y + scaledOffset.dimensions.y;
        return Mesh(Mesh::TRI, {
                Vertex(Vec3f(0 - center.x, 0 - center.y, 0), {uvNearX, uvNearY}),
                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0), {uvFarX, uvNearY}),
                Vertex(Vec3f(0 - center.x, size.y - center.y, 0), {uvNearX, uvFarY}),
                Vertex(Vec3f(0 - center.x, size.y - center.y, 0), {uvNearX, uvFarY}),
                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0), {uvFarX, uvNearY}),
                Vertex(Vec3f(size.x - center.x, size.y - center.y, 0), {uvFarX, uvFarY})
        });
    }

    static Mesh getSquare(Vec2f size, Vec2f center) {
        return Mesh(Mesh::LINE, {
                Vertex(Vec3f(0 - center.x, 0 - center.y, 0)),
                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0)),

                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0)),
                Vertex(Vec3f(size.x - center.x, size.y - center.y, 0)),

                Vertex(Vec3f(size.x - center.x, size.y - center.y, 0)),
                Vertex(Vec3f(0 - center.x, size.y - center.y, 0)),

                Vertex(Vec3f(0 - center.x, size.y - center.y, 0)),
                Vertex(Vec3f(0 - center.x, 0 - center.y, 0))
        });
    }

    static Mesh getLine(Vec2f start, Vec2f end, Vec2f center) {
        return Mesh(Mesh::LINE, {
                Vertex(Vec3f(start.x - center.x, start.y - center.y, 0)),
                Vertex(Vec3f(end.x - center.x, end.y - center.y, 0))
        });
    }

    Renderer2D::Renderer2D() = default;

    Renderer2D::Renderer2D(RenderDevice &device) : renderDevice(&device) {
        defaultShader = device.getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {}, includeCallback);
        defaultTextShader = device.getAllocator().createShaderProgram(SHADER_VERT, SHADER_TEXT_FRAG, {},
                                                                      includeCallback);
    }

    Renderer2D::~Renderer2D() {
        delete defaultTextShader;
        delete defaultShader;
    }

    Renderer2D::Renderer2D(const Renderer2D &other) {
        defaultShader = other.renderDevice->getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {},
                                                                               includeCallback);
        defaultTextShader = other.renderDevice->getAllocator().createShaderProgram(SHADER_VERT, SHADER_TEXT_FRAG, {},
                                                                                   includeCallback);
        renderDevice = other.renderDevice;
    }

    Renderer2D &Renderer2D::operator=(const Renderer2D &other) {
        if (this == &other)
            return *this;

        defaultShader = other.renderDevice->getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {},
                                                                               includeCallback);
        defaultTextShader = other.renderDevice->getAllocator().createShaderProgram(SHADER_VERT, SHADER_TEXT_FRAG, {},
                                                                                   includeCallback);

        renderDevice = other.renderDevice;

        return *this;
    }

    void Renderer2D::renderBegin(RenderTarget &target, bool clear) {
        renderDevice->getRenderer().renderBegin(target, RenderOptions({},
                                                                      target.getSize(),
                                                                      true,
                                                                      {},
                                                                      clear, clear, clear));
        screenSize = target.getSize();
        setProjection({{}, screenSize.convert<float>()});
    }

    void Renderer2D::renderBegin(RenderTarget &target,
                                 bool clear,
                                 Vec2i viewportOffset,
                                 Vec2i viewportSize) {
        renderDevice->getRenderer().renderBegin(target, RenderOptions(viewportOffset,
                                                                      viewportSize,
                                                                      true,
                                                                      {},
                                                                      clear, clear, clear));
        screenSize = viewportSize;
        setProjection({{}, screenSize.convert<float>()});
    }


    void Renderer2D::setProjection(const Rectf &projection) {
        camera.type = ORTHOGRAPHIC;
        camera.transform.position = {0, 0, 1};
        camera.left = projection.position.x;
        camera.right = projection.dimensions.x;
        camera.top = projection.position.y;
        camera.bottom = projection.dimensions.y;
    }

    void Renderer2D::draw(Rectf srcRect,
                          Rectf dstRect,
                          TextureBuffer &texture,
                          ShaderProgram &shader,
                          Vec2f center,
                          float rotation) {
        Mesh mesh = getPlane(dstRect.dimensions, center, srcRect);

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        RenderCommand command;
        command.properties.enableDepthTest = false;
        command.properties.enableBlending = true;
        command.shader = &shader;
        command.meshBuffers.emplace_back(buffer);
        command.textures.emplace_back(&texture);

        Mat4f modelMatrix(1);
        modelMatrix = modelMatrix * MatrixMath::translate(Vec3f(
                dstRect.position.x + center.x,
                dstRect.position.y + center.y,
                0));
        modelMatrix = modelMatrix * MatrixMath::rotate(Vec3f(0, 0, rotation));

        modelMatrix = camera.projection() * camera.view() * modelMatrix;

        command.shader->setMat4("MODEL_MATRIX", modelMatrix);
        command.shader->setFloat("USE_TEXTURE", 1);
        command.shader->setVec4("COLOR", Vec4f(1, 1, 1, 1));

        command.shader->setTexture("diffuse", 0);

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, TextureBuffer &texture, Vec2f center, float rotation) {
        draw(srcRect, dstRect, texture, *defaultShader, center, rotation);
    }

    void Renderer2D::draw(Rectf dstRect, TextureBuffer &texture, Vec2f center, float rotation) {
        draw(Rectf({}, dstRect.dimensions), dstRect, texture, center, rotation);
    }

    void Renderer2D::draw(Rectf rectangle, ColorRGBA color, bool fill, Vec2f center, float rotation) {
        Mesh mesh;

        if (fill)
            mesh = getPlane(rectangle.dimensions, center, Rectf(Vec2f(), rectangle.dimensions));
        else
            mesh = getSquare(rectangle.dimensions, center);

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        RenderCommand command;
        command.properties.enableDepthTest = false;
        command.properties.enableBlending = true;
        command.shader = defaultShader;
        command.meshBuffers.emplace_back(buffer);

        Mat4f modelMatrix(1);
        modelMatrix = modelMatrix * MatrixMath::translate(Vec3f(
                rectangle.position.x + center.x,
                rectangle.position.y + center.y,
                0));
        modelMatrix = modelMatrix * MatrixMath::rotate(Vec3f(0, 0, rotation));

        modelMatrix = camera.projection() * camera.view() * modelMatrix;

        command.shader->setMat4("MODEL_MATRIX", modelMatrix);
        command.shader->setFloat("USE_TEXTURE", 0);
        command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                               (float) color.g() / 255,
                                               (float) color.b() / 255,
                                               (float) color.a() / 255));

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Vec2f start,
                          Vec2f end,
                          ColorRGBA color,
                          Vec2f center,
                          float rotation) {
        Mesh mesh = getLine(start, end, center);

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        RenderCommand command;
        command.properties.enableDepthTest = false;
        command.properties.enableBlending = true;
        command.shader = defaultShader;
        command.meshBuffers.emplace_back(buffer);

        Mat4f modelMatrix(1);
        modelMatrix = modelMatrix * MatrixMath::rotate(Vec3f(0, 0, rotation));

        modelMatrix = camera.projection() * camera.view() * modelMatrix;

        command.shader->setMat4("MODEL_MATRIX", modelMatrix);
        command.shader->setFloat("USE_TEXTURE", 0);
        command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                               (float) color.g() / 255,
                                               (float) color.b() / 255,
                                               (float) color.a() / 255));

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Vec2f point, ColorRGBA color) {
        Mesh mesh(Mesh::POINT, {
                Vertex(Vec3f(point.x, point.y, 0))
        });

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        RenderCommand command;
        command.properties.enableDepthTest = false;
        command.properties.enableBlending = true;
        command.shader = defaultShader;
        command.meshBuffers.emplace_back(buffer);

        Mat4f modelMatrix(1);
        modelMatrix = camera.projection() * camera.view() * modelMatrix;

        command.shader->setMat4("MODEL_MATRIX", modelMatrix);
        command.shader->setFloat("USE_TEXTURE", 0);
        command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                               (float) color.g() / 255,
                                               (float) color.b() / 255,
                                               (float) color.a() / 255));

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Vec2f position,
                          Vec2f scale,
                          const std::string &text, std::map<char, Character> &mapping,
                          ColorRGBA color) {
        float x = position.x;
        float y = position.y;

        for (auto &c : text) {
            auto &character = mapping.at(c);

            float xpos = (x + static_cast<float>(character.getBearing().x)) * scale.x;
            float ypos = (y - static_cast<float>(character.getBearing().y)) * scale.y;

            float w = static_cast<float>(character.getSize().x) * scale.x;
            float h = static_cast<float>(character.getSize().y) * scale.y;

            x += static_cast<float>(static_cast<float>(character.getAdvance()) * scale.x);

            Mesh mesh = getPlane(Vec2f(w, h), Vec2f(), Rectf(Vec2f(), Vec2f(w, h)));

            auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
            allocatedMeshes.insert(buffer);

            RenderCommand command;
            command.properties.enableDepthTest = false;
            command.properties.enableBlending = true;
            command.shader = defaultTextShader;
            command.meshBuffers.emplace_back(buffer);
            command.textures.emplace_back(&character.getTexture());

            Mat4f modelMatrix(1);
            modelMatrix = modelMatrix * MatrixMath::translate(Vec3f(xpos, ypos, 0));
            modelMatrix = camera.projection() * camera.view() * modelMatrix;

            command.shader->setMat4("MODEL_MATRIX", modelMatrix);
            command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                                   (float) color.g() / 255,
                                                   (float) color.b() / 255,
                                                   (float) color.a() / 255));

            command.shader->setTexture("diffuse", 0);

            renderDevice->getRenderer().addCommand(command);
        }
    }

    void Renderer2D::renderPresent() {
        renderDevice->getRenderer().renderFinish();
        for (auto *mesh : allocatedMeshes)
            delete mesh;
        allocatedMeshes.clear();
    }
}