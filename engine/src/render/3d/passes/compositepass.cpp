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

#include "engine/render/3d/passes/compositepass.hpp"

static const char *SHADER_VERT = R"###(
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

static const char *SHADER_FRAG = R"###(
struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 color     :   SV_TARGET0;
    float depth       :   SV_Depth;
};

Texture2D depth;
Texture2D phong_ambient;
Texture2D phong_diffuse;
Texture2D phong_specular;

Texture2D forward;
Texture2D forward_depth;

Texture2D skybox;

SamplerState samplerState_depth
{};
SamplerState samplerState_phong_ambient
{};
SamplerState samplerState_phong_diffuse
{};
SamplerState samplerState_phong_specular
{};

SamplerState samplerState_forward
{};
SamplerState samplerState_forward_depth
{};

SamplerState samplerState_skybox
{};

float LinearizeDepth(float depth)
{
    float near = 0.1;
    float far  = 100.0;
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    float4 ambient = phong_ambient.Sample(samplerState_phong_ambient, v.fUv);
    float4 diffuse = phong_diffuse.Sample(samplerState_phong_diffuse, v.fUv);
    float4 specular = phong_specular.Sample(samplerState_phong_specular, v.fUv);

    float deferredDepth = depth.Sample(samplerState_depth, v.fUv).r;
    float forwardDepth = forward_depth.Sample(samplerState_forward_depth, v.fUv).r;

    float4 colorForward = forward.Sample(samplerState_forward, v.fUv);

    float4 colorSkybox = skybox.Sample(samplerState_skybox, v.fUv);

    if (deferredDepth == 1
        && forwardDepth == 1)
    {
        ret.color = colorSkybox;
        ret.depth = 0.0f;
    }
    else if (deferredDepth < forwardDepth)
    {
        ret.color =  ambient + diffuse + specular;
        ret.depth = deferredDepth;
    }
    else
    {
        ret.color = colorForward;
        ret.depth = forwardDepth;
    }

    return ret;
}
)###";

namespace engine {
    CompositePass::CompositePass(RenderDevice &device)
            : device(device) {
        shader = device.getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {}, {});
    }

    CompositePass::~CompositePass() {
        delete shader;
    }

    void CompositePass::presentBuffer(RenderTarget &screen, GeometryBuffer &buffer) {
        shader->setTexture("depth", 0);
        shader->setTexture("phong_ambient", 1);
        shader->setTexture("phong_diffuse", 2);
        shader->setTexture("phong_specular", 3);
        shader->setTexture("forward", 4);
        shader->setTexture("forward_depth", 5);
        shader->setTexture("skybox", 6);

        RenderCommand command;
        command.shader = shader;

        command.textures.emplace_back(&buffer.getBuffer("depth"));
        command.textures.emplace_back(&buffer.getBuffer("phong_ambient"));
        command.textures.emplace_back(&buffer.getBuffer("phong_diffuse"));
        command.textures.emplace_back(&buffer.getBuffer("phong_specular"));
        command.textures.emplace_back(&buffer.getBuffer("forward"));
        command.textures.emplace_back(&buffer.getBuffer("forward_depth"));
        command.textures.emplace_back(&buffer.getBuffer("skybox"));

        command.meshBuffers.emplace_back(&buffer.getScreenQuad());

        command.properties.depthTestMode = DEPTH_TEST_ALWAYS;
        command.properties.enableBlending = true;

        auto &ren = device.getRenderer();
        ren.renderBegin(screen, RenderOptions({}, screen.getSize()));
        ren.addCommand(command);
        ren.renderFinish();
    }
}