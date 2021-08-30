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

#include "engine/render/3d/passes/lightingpass.hpp"

#include "engine/render/3d/renderer3d.hpp"

#include "engine/math/rotation.hpp"

const char *SHADER_VERT_LIGHTING = R"###(
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
    float4 FragColor: SV_TARGET;
};

Texture2D position;
Texture2D normal;
Texture2D diffuse;
Texture2D ambient;
Texture2D specular;
Texture2D lighting;
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

SamplerState samplerState_lighting
{};

SamplerState samplerState_depth
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    float3 fragPos = position.Sample(samplerState_position, v.fUv).xyz;
    float3 fragNorm = normal.Sample(samplerState_normal, v.fUv).xyz;
    float4 fragDiffuse = diffuse.Sample(samplerState_diffuse, v.fUv);
    float4 fragSpecular = specular.Sample(samplerState_specular, v.fUv);
    float fragShininess = lighting.Sample(samplerState_lighting, v.fUv).r;
    float fragDepth = depth.Sample(samplerState_depth, v.fUv).r;
    float influence = lighting.Sample(samplerState_lighting, v.fUv).g;
    if (influence == 0)
        ret.FragColor = fragDiffuse;
    else
        ret.FragColor = mana_calculate_light(fragPos,
                                                fragNorm,
                                                fragDiffuse,
                                                fragSpecular,
                                                fragShininess);
    return ret;
}
)###";

namespace mana {
    LightingPass::LightingPass(RenderDevice &device)
            : renderDevice(device) {
        auto &allocator = device.getAllocator();

        shader = allocator.createShaderProgram(SHADER_VERT_LIGHTING,
                                               SHADER_FRAG_LIGHTING,
                                               Renderer3D::getShaderMacros(),
                                               Renderer3D::getShaderIncludeCallback());
    }

    void LightingPass::render(RenderTarget &screen, RenderScene &scene, GeometryBuffer &gBuffer) {
        //Render screen quad, calculate lighting, store result in screen buffer overwriting existing values.
        int dirCount = 0;
        int pointCount = 0;
        int spotCount = 0;

        int i = 0;
        for (auto &light : scene.lights) {
            std::string name;
            switch (light.type) {
                case LIGHT_DIRECTIONAL:
                    name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                    shader->setVec3(name + "direction", light.direction);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    dirCount++;
                    break;
                case LIGHT_POINT:
                    name = "MANA_LIGHTS_POINT[" + std::to_string(i++) + "].";
                    shader->setVec3(name + "position", light.transform.position);
                    shader->setFloat(name + "constantValue", light.constant);
                    shader->setFloat(name + "linearValue", light.linear);
                    shader->setFloat(name + "quadraticValue", light.quadratic);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    pointCount++;
                    break;
                case LIGHT_SPOT:
                    name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
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
                    spotCount++;
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
        command.shader->setTexture("lighting", 5);
        command.shader->setTexture("depth", 6);

        command.textures.emplace_back(&gBuffer.getPosition());
        command.textures.emplace_back(&gBuffer.getNormal());
        command.textures.emplace_back(&gBuffer.getDiffuse());
        command.textures.emplace_back(&gBuffer.getAmbient());
        command.textures.emplace_back(&gBuffer.getSpecular());
        command.textures.emplace_back(&gBuffer.getLighting());
        command.textures.emplace_back(&gBuffer.getDepthStencil());

        command.properties.enableDepthTest = false;
        command.properties.enableStencilTest = false;
        command.properties.enableFaceCulling = false;
        command.properties.enableBlending = false;

        auto &ren = renderDevice.getRenderer();

        ren.renderBegin(screen, RenderOptions({}, screen.getSize(), true, {255, 0, 0, 255}));
        ren.addCommand(command);
        ren.renderFinish();
    }
}