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

#include "engine/render/3d/passes/phongshadepass.hpp"

#include "engine/render/3d/renderer3d.hpp"

#include "engine/math/rotation.hpp"

#include "engine/render/shadercompiler.hpp"

const char *SHADER_VERT_LIGHTING = R"###(
struct VS_INPUT
{
    float3 position : POSITION0;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent: TANGENT;
    float3 bitangent: BINORMAL;
    float4 instanceRow0 : POSITION1;
    float4 instanceRow1 : POSITION2;
    float4 instanceRow2 : POSITION3;
    float4 instanceRow3 : POSITION4;
};

struct VS_OUTPUT
{
    float3  fPos : POSITION;
    float3  fNorm : NORMAL;
    float2  fUv : TEXCOORD0;
    float4 vPos : SV_Position;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;

    ret.vPos = float4(v.position, 1);
    ret.fPos = v.position;
    ret.fNorm = v.normal;
    ret.fUv = v.uv;

    return ret;
}
)###";

const char *SHADER_FRAG_LIGHTING = R"###(
#include "mana.hlsl"

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 phong_ambient: SV_TARGET0;
    float4 phong_diffuse: SV_TARGET1;
    float4 phong_specular: SV_TARGET2;
};

Texture2D position;
Texture2D normal;
Texture2D diffuse;
Texture2D ambient;
Texture2D specular;
Texture2D shininess;
Texture2D depth;

SamplerState samplerState_position
{};

SamplerState samplerState_normal
{};

SamplerState samplerState_diffuse
{};

SamplerState samplerState_ambient
{};

SamplerState samplerState_specular
{};

SamplerState samplerState_shininess
{};

SamplerState samplerState_depth
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;

    float3 fragPos = position.Sample(samplerState_position, v.fUv).xyz;
    float3 fragNorm = normal.Sample(samplerState_normal, v.fUv).xyz;
    float4 fragDiffuse = diffuse.Sample(samplerState_diffuse, v.fUv);
    float4 fragSpecular = specular.Sample(samplerState_specular, v.fUv);
    float fragShininess = shininess.Sample(samplerState_shininess, v.fUv).r;

    float fragDepth = depth.Sample(samplerState_depth, v.fUv).r;

    LightComponents comp = mana_calculate_light(fragPos,
                                                fragNorm,
                                                fragDiffuse,
                                                fragSpecular,
                                                fragShininess);

    ret.phong_ambient = float4(comp.ambient, 1);
    ret.phong_diffuse = float4(comp.diffuse, 1);
    ret.phong_specular = float4(comp.specular, 1);

    return ret;
}
)###";

namespace engine {
    using namespace ShaderCompiler;

    PhongShadePass::PhongShadePass(RenderDevice &device)
            : renderDevice(device) {
        ShaderSource vertexShader(SHADER_VERT_LIGHTING,
                                  "main",
                                  VERTEX,
                                  HLSL);
        ShaderSource fragmentShader(SHADER_FRAG_LIGHTING,
                                    "main",
                                    FRAGMENT,
                                    HLSL);

        vertexShader.preprocess(Renderer3D::getShaderIncludeCallback(), Renderer3D::getShaderMacros(HLSL));
        fragmentShader.preprocess(Renderer3D::getShaderIncludeCallback(), Renderer3D::getShaderMacros(HLSL));

        auto &allocator = device.getAllocator();

        shader = allocator.createShaderProgram(vertexShader, fragmentShader);
    }

    void PhongShadePass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer("phong_ambient", TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer("phong_diffuse", TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer("phong_specular", TextureBuffer::ColorFormat::RGBA);
    }

    void PhongShadePass::render(GeometryBuffer &gBuffer, RenderScene &scene) {
        int dirCount = 0;
        int pointCount = 0;
        int spotCount = 0;

        for (auto &light : scene.lights) {
            std::string name;
            switch (light.type) {
                case LIGHT_DIRECTIONAL:
                    name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(dirCount++) + "].";
                    shader->setVec3(name + "direction", light.direction);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
                case LIGHT_POINT:
                    name = "MANA_LIGHTS_POINT[" + std::to_string(pointCount++) + "].";
                    shader->setVec3(name + "position", light.transform.position);
                    shader->setFloat(name + "constantValue", light.constant);
                    shader->setFloat(name + "linearValue", light.linear);
                    shader->setFloat(name + "quadraticValue", light.quadratic);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
                case LIGHT_SPOT:
                    name = "MANA_LIGHTS_SPOT[" + std::to_string(spotCount++) + "].";
                    shader->setVec3(name + "position", light.transform.position);
                    shader->setVec3(name + "direction", light.direction);
                    shader->setFloat(name + "cutOff", cosf(degreesToRadians(light.cutOff)));
                    shader->setFloat(name + "outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                    shader->setFloat(name + "constantValue", light.constant);
                    shader->setFloat(name + "linearValue", light.linear);
                    shader->setFloat(name + "quadraticValue", light.quadratic);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
            }
        }

        shader->setInt("MANA_LIGHT_COUNT_DIRECTIONAL", dirCount);
        shader->setInt("MANA_LIGHT_COUNT_POINT", pointCount);
        shader->setInt("MANA_LIGHT_COUNT_SPOT", spotCount);

        shader->setVec3("MANA_VIEWPOS", scene.camera.transform.position);

        RenderCommand command;

        command.meshBuffers.emplace_back(&gBuffer.getScreenQuad());

        command.shader = shader;

        command.shader->setTexture("position", 0);
        command.shader->setTexture("normal", 1);
        command.shader->setTexture("diffuse", 2);
        command.shader->setTexture("ambient", 3);
        command.shader->setTexture("specular", 4);
        command.shader->setTexture("shininess", 5);
        command.shader->setTexture("depth", 6);

        command.textures.emplace_back(&gBuffer.getBuffer("position"));
        command.textures.emplace_back(&gBuffer.getBuffer("normal"));
        command.textures.emplace_back(&gBuffer.getBuffer("diffuse"));
        command.textures.emplace_back(&gBuffer.getBuffer("ambient"));
        command.textures.emplace_back(&gBuffer.getBuffer("specular"));
        command.textures.emplace_back(&gBuffer.getBuffer("shininess"));
        command.textures.emplace_back(&gBuffer.getBuffer("depth"));

        command.properties.enableDepthTest = false;
        command.properties.enableStencilTest = false;
        command.properties.enableFaceCulling = false;
        command.properties.enableBlending = false;

        auto &ren = renderDevice.getRenderer();

        gBuffer.attachColor({"phong_ambient", "phong_diffuse", "phong_specular"});

        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getSize()));
        ren.addCommand(command);
        ren.renderFinish();
    }
}