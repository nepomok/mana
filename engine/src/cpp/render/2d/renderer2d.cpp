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
Texture2D content;

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    if (USE_TEXTURE)
        ret.pixel = diffuse.Sample(samplerState_diffuse, v.uv);
    else
        ret.pixel = COLOR;
    return ret;
}
)###";

static std::string includeCallback(const char *name) {
    return SHADER_GLOBALS;
}

namespace mana {
    /**
     * Get plane mesh with origin at top left offset by center and scaled in the y axis.
     *
     * @param size
     * @return
     */
    static Mesh getPlane(Vec2f size, Vec2f center) {
        return Mesh(Mesh::TRI, {
                Vertex(Vec3f(0 - center.x, 0 - center.y, 0)),
                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0)),
                Vertex(Vec3f(0 - center.x, size.y - center.y, 0)),
                Vertex(Vec3f(0 - center.x, size.y - center.y, 0)),
                Vertex(Vec3f(size.x - center.x, 0 - center.y, 0)),
                Vertex(Vec3f(size.x - center.x, size.y - center.y, 0))
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

    Renderer2D::Renderer2D() = default;

    Renderer2D::Renderer2D(RenderDevice &device) : renderDevice(&device) {
        defaultShader = device.getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {}, includeCallback);
    }

    void Renderer2D::renderBegin(RenderTarget &target, bool clear) {
        renderDevice->getRenderer().renderBegin(target, RenderOptions({},
                                                                      target.getSize(),
                                                                      true,
                                                                      {},
                                                                      clear, clear, clear));
        screenSize = target.getSize();
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
    }

    void Renderer2D::draw(Rectf srcRect,
                          Rectf dstRect,
                          TextureBuffer &texture,
                          ShaderProgram &shader,
                          Vec2f center,
                          float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, TextureBuffer &texture, ShaderProgram &shader) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, TextureBuffer &texture, Vec2f center, float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Rectf srcRect, Rectf dstRect, TextureBuffer &texture) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Rectf rectangle, ColorRGBA color, bool fill, Vec2f center, float rotation) {
        Mesh mesh;

        if (fill)
            mesh = getPlane(rectangle.dimensions, center);
        else
            mesh = getSquare(rectangle.dimensions, center);

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        Camera camera;
        camera.type = ORTHOGRAPHIC;
        camera.transform.position = {0, 0, 1};
        camera.left = 0;
        camera.right = 1;
        camera.top = 0;
        camera.bottom = 1;

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
        command.shader->setFloat("USE_TEXTURE", false);
        command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                               (float) color.g() / 255,
                                               (float) color.b() / 255,
                                               (float) color.a() / 255));

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Rectf rectangle, ColorRGBA color, bool fill) {
        draw(rectangle, color, fill, {}, 45);
    }

    void Renderer2D::draw(Vec2f start, Vec2f end, ColorRGBA color, Vec2f center, float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2f start, Vec2f end, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2f point, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Recti srcRect,
                          Recti dstRect,
                          TextureBuffer &texture,
                          ShaderProgram &shader,
                          Vec2i center,
                          float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Recti srcRect,
                          Recti dstRect,
                          TextureBuffer &texture,
                          ShaderProgram &shader) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, TextureBuffer &texture, Vec2i center, float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Recti srcRect, Recti dstRect, TextureBuffer &texture) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Recti rectangle, ColorRGBA color, bool fill, Vec2i center, float rotation) {
        Mesh mesh;

        if (fill)
            mesh = getPlane(Vec2f(rectangle.dimensions.x, rectangle.dimensions.y), Vec2f(center.x, center.y));
        else
            mesh = getSquare(Vec2f(rectangle.dimensions.x, rectangle.dimensions.y), Vec2f(center.x, center.y));

        auto buffer = renderDevice->getAllocator().createMeshBuffer(mesh);
        allocatedMeshes.insert(buffer);

        Camera camera;
        camera.type = ORTHOGRAPHIC;
        camera.transform.position = {0, 0, 1};
        camera.left = 0;
        camera.right = screenSize.x;
        camera.top = 0;
        camera.bottom = screenSize.y;

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
        command.shader->setFloat("USE_TEXTURE", false);
        command.shader->setVec4("COLOR", Vec4f((float) color.r() / 255,
                                               (float) color.g() / 255,
                                               (float) color.b() / 255,
                                               (float) color.a() / 255));

        renderDevice->getRenderer().addCommand(command);
    }

    void Renderer2D::draw(Recti rectangle, ColorRGBA color, bool fill) {
        draw(rectangle, color, fill, {}, 0);
    }

    void Renderer2D::draw(Vec2i start, Vec2i end, ColorRGBA color, Vec2i center, float rotation) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2i start, Vec2i end, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2i point, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2i pos, const std::string &text, std::map<char, Character> &mapping, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2f pos, const std::string &text, std::map<char, Character> &mapping, ColorRGBA color) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2i pos,
                          const std::string &text,
                          std::map<char, Character> &mapping,
                          ShaderProgram *shader) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::draw(Vec2f pos,
                          const std::string &text,
                          std::map<char, Character> &mapping,
                          ShaderProgram *shader) {
        throw std::runtime_error("Not Implemented");
    }

    void Renderer2D::renderPresent() {
        renderDevice->getRenderer().renderFinish();
        for (auto *mesh : allocatedMeshes)
            delete mesh;
        allocatedMeshes.clear();
    }
}