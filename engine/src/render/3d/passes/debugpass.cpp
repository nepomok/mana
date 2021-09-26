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

#include "engine/render/3d/passes/debugpass.hpp"

#include "engine/render/3d/renderer3d.hpp"

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
    float4 vPos : SV_Position;
    float3 vNorm : NORMAL;
    float3 vTang : TANGENT;
    float3 vBitang : BINORMAL;
};

float4x4 mvp;
float scale;

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;
    ret.vPos = float4(v.position, 1);
    ret.vNorm = v.normal;
    ret.vTang = v.tangent;
    ret.vBitang = v.bitangent;
    return ret;
}
)###";

static const char *SHADER_GEOMETRY = R"###(
struct GS_INPUT
{
    float4 vPos : SV_Position;
    float3 vNorm : NORMAL;
    float3 vTang : TANGENT;
    float3 vBitang : BINORMAL;
};

struct GS_OUTPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

float4x4 mvp;
float scale;

[maxvertexcount(18)]
void main(triangle GS_INPUT input[3], inout LineStream<GS_OUTPUT> output)
{
    GS_OUTPUT o;

    [unroll(3)]
    for(uint i = 0; i < 3; i++)
    {
        // Draw line for normal, tangent and bitangent
        GS_INPUT vertex = input[i];

        //Normal
        o.color = float4(0, 0, 1, 1);
        o.pos = mul(vertex.vPos, mvp);
        output.Append(o);
        o.pos = mul(float4(vertex.vNorm, 0) * scale + vertex.vPos, mvp);
        output.Append(o);
        output.RestartStrip();

        //Tangent
        o.color = float4(1, 0, 0, 1);
        o.pos = mul(vertex.vPos, mvp);
        output.Append(o);
        o.pos = mul(float4(vertex.vTang, 0) * scale + vertex.vPos, mvp);
        output.Append(o);
        output.RestartStrip();

        //Bitangent
        o.color = float4(0, 1, 0, 1);
        o.pos = mul(vertex.vPos, mvp);
        output.Append(o);
        o.pos = mul(float4(vertex.vBitang, 0) * scale + vertex.vPos, mvp);
        output.Append(o);
        output.RestartStrip();
    }
}
)###";

static const char *SHADER_FRAG = R"###(
struct PS_INPUT
{
    float4 color    : COLOR;
};
struct PS_OUTPUT
{
    float4 color     :   SV_TARGET0;
};

float4x4 mvp;
float scale;

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT ret;
    ret.color = input.color;
    return ret;
}
)###";

namespace engine {
    DebugPass::DebugPass(RenderDevice &device)
            : device(device),
              drawNormals(false) {
        shader = device.getAllocator().createShaderProgram(SHADER_VERT,
                                                           SHADER_GEOMETRY,
                                                           SHADER_FRAG,
                                                           Renderer3D::getShaderMacros(),
                                                           Renderer3D::getShaderIncludeCallback());
    }

    DebugPass::~DebugPass() {
        delete shader;
    }

    void DebugPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer("debug", TextureBuffer::RGBA);
    }

    void DebugPass::render(GeometryBuffer &gBuffer, RenderScene &scene) {
        gBuffer.attachColor({"debug"});
        gBuffer.attachDepthStencil("depth");

        auto &ren = device.getRenderer();

        ren.renderBegin(gBuffer.getRenderTarget(),
                        RenderOptions({},
                                      gBuffer.getSize(),
                                      false,
                                      {},
                                      true,
                                      false,
                                      false));

        if (drawNormals) {
            for (auto &deferredCommand : scene.deferred) {
                shader->setMat4("mvp",
                                scene.camera.projection() * scene.camera.view() * deferredCommand.transform.model());
                shader->setVec4("color", Vec4f(1, 1, 0, 1));
                shader->setFloat("scale", 0.1f);

                RenderCommand command;
                command.shader = shader;
                command.meshBuffers.emplace_back(deferredCommand.meshBuffer);
                if (deferredCommand.material.normalTexture != nullptr) {}
                command.properties.depthTestWrite = false;

                ren.addCommand(command);
            }
        }

        ren.renderFinish();
    }

    void DebugPass::setDrawNormals(bool value) {
        drawNormals = value;
    }
}