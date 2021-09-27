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
    float2 vUv     : TEXCOORD;
};

float4x4 MODEL;
float4x4 VIEW;
float4x4 PROJECTION;
float scale;
int hasNormalTexture;

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;
    ret.vPos = float4(v.position, 1);
    ret.vNorm = v.normal;
    ret.vTang = v.tangent;
    ret.vBitang = v.bitangent;
    ret.vUv = v.uv;
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
    float2 vUv  : TEXCOORD;
};

struct GS_OUTPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

float4x4 MODEL;
float4x4 VIEW;
float4x4 PROJECTION;
float scale;
int hasNormalTexture;

Texture2D normal;
SamplerState samplerState_normal
{};

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

[maxvertexcount(18)]
void main(triangle GS_INPUT input[3], inout LineStream<GS_OUTPUT> output)
{
    GS_OUTPUT o;

    float4x4 mv = mul(MODEL, VIEW);
    float4x4 mvp = mul(mv, PROJECTION);

    [unroll(3)]
    for(uint i = 0; i < 3; i++)
    {
        // Draw line for normal, tangent and bitangent
        GS_INPUT vertex = input[i];

        if (hasNormalTexture)
        {
            // Draw texture normals at vertex positions, normals for fragments between vertices are not drawn
            float3 T = normalize(vertex.vTang);
            float3 B = normalize(vertex.vBitang);
            float3 N = normalize(vertex.vNorm);

            float3x3 TBN = float3x3(T, B, N);

            float3 tangentNormal = normal.Sample(samplerState_normal, vertex.vUv);
            tangentNormal = normalize((tangentNormal * 2) - 1);

            //Transform tangent space normal into local space
            float3 localNormal = mul(tangentNormal * scale, TBN);

            //Transform local space normal into clip space
            float4 clipNormal = mul(float4((float4(localNormal, 1) + vertex.vPos).xyz, 1), mvp);
            float4 clipPos = mul(vertex.vPos, mvp);

            o.color = float4(1, 1, 0, 1);
            o.pos = clipPos;
            output.Append(o);
            o.pos = clipNormal;
            output.Append(o);
            output.RestartStrip();
        }
        else
        {
            float4 cPos = mul(vertex.vPos, mvp);
            float4 cNorm = mul(float4((float4(vertex.vNorm * scale, 0) + vertex.vPos).xyz, 1), mvp);
            float4 cTang = mul(float4((float4(vertex.vTang * scale, 0) + vertex.vPos).xyz, 1), mvp);
            float4 cBitang = mul(float4((float4(vertex.vBitang * scale, 0) + vertex.vPos).xyz, 1), mvp);

            // Draw vertex normal, tangent and bitangent
            //Normal
            o.color = float4(0, 0, 1, 1);
            o.pos = cPos;
            output.Append(o);
            o.pos = cNorm;
            output.Append(o);
            output.RestartStrip();

            //Tangent
            o.color = float4(1, 0, 0, 1);
            o.pos = cPos;
            output.Append(o);
            o.pos = cTang;
            output.Append(o);
            output.RestartStrip();

            //Bitangent
            o.color = float4(0, 1, 0, 1);
            o.pos = cPos;
            output.Append(o);
            o.pos = cBitang;
            output.Append(o);
            output.RestartStrip();
        }
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

float4x4 MODEL;
float4x4 VIEW;
float4x4 PROJECTION;
float scale;
int hasNormalTexture;

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
                shader->setVec4("color", Vec4f(1, 1, 0, 1));
                shader->setFloat("scale", 0.1f);
                shader->setMat4("MODEL", deferredCommand.transform.model());
                shader->setMat4("VIEW", scene.camera.view());
                shader->setMat4("PROJECTION", scene.camera.projection());

                RenderCommand command;
                command.shader = shader;
                command.meshBuffers.emplace_back(deferredCommand.meshBuffer);
                if (deferredCommand.material.normalTexture != nullptr) {}
                command.properties.depthTestWrite = false;

                if (deferredCommand.material.normalTexture != nullptr) {
                    shader->setBool("hasNormalTexture", true);
                    shader->setTexture("normal", 0);
                    command.textures.emplace_back(deferredCommand.material.normalTexture);
                } else {
                    shader->setBool("hasNormalTexture", false);
                }

                ren.addCommand(command);
            }
        }

        ren.renderFinish();
    }

    void DebugPass::setDrawNormals(bool value) {
        drawNormals = value;
    }
}