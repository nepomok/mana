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

#include "engine/render/3d/passes/geometrypass.hpp"

#include "engine/render/3d/renderer3d.hpp"

const char *SHADER_VERT_GEOMETRY = R"###(
#include "mana.hlsl"

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

    float4x4 instanceMatrix = float4x4(v.instanceRow0, v.instanceRow1, v.instanceRow2, v.instanceRow3);

    ret.vPos = mul(float4(v.position, 1), mul(instanceMatrix, MANA_MVP));
    ret.fPos = mul(float4(v.position, 1), mul(instanceMatrix, MANA_M)).xyz;
    ret.fNorm = mul(float4(v.normal, 1), transpose(mul(MANA_M_INVERT, instanceMatrix))).xyz;
    ret.fUv = v.uv;

    return ret;
}
)###";

const char *SHADER_FRAG_GEOMETRY_TEXTURENORMALS = R"###(
struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
     float4 position     :   SV_TARGET0;
     float4 normal       :   SV_TARGET1;
     float4 diffuse      :   SV_TARGET2;
     float4 ambient      :   SV_TARGET3;
     float4 specular     :   SV_TARGET4;
     float4 shininess    :   SV_TARGET5;
};

Texture2D diffuse;
Texture2D ambient;
Texture2D specular;
Texture2D shininess;
Texture2D normal;
Texture2D emissive;

SamplerState samplerState_diffuse
{};
SamplerState samplerState_ambient
{};
SamplerState samplerState_specular
{};
SamplerState samplerState_shininess
{};
SamplerState samplerState_normal
{};
SamplerState samplerState_emissive
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.position = float4(v.fPos, 1);
    ret.normal = normal.Sample(samplerState_normal, v.fUv);
    ret.diffuse = diffuse.Sample(samplerState_diffuse, v.fUv);
    ret.ambient = ambient.Sample(samplerState_ambient, v.fUv);
    ret.specular = specular.Sample(samplerState_specular, v.fUv);
    ret.shininess = shininess.Sample(samplerState_shininess, v.fUv);
    return ret;
}
)###";

const char *SHADER_FRAG_GEOMETRY_VERTEXNORMALS = R"###(
struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
     float4 position     :   SV_TARGET0;
     float4 normal       :   SV_TARGET1;
     float4 diffuse      :   SV_TARGET2;
     float4 ambient      :   SV_TARGET3;
     float4 specular     :   SV_TARGET4;
     float4 shininess    :   SV_TARGET5;
};

Texture2D diffuse;
Texture2D ambient;
Texture2D specular;
Texture2D shininess;
Texture2D emissive;

SamplerState samplerState_diffuse
{};
SamplerState samplerState_ambient
{};
SamplerState samplerState_specular
{};
SamplerState samplerState_shininess
{};
SamplerState samplerState_emissive
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.position = float4(v.fPos, 1);
    ret.normal = float4(v.fNorm, 1);
    ret.diffuse = diffuse.Sample(samplerState_diffuse, v.fUv);
    ret.ambient = ambient.Sample(samplerState_ambient, v.fUv);
    ret.specular = specular.Sample(samplerState_specular, v.fUv);
    ret.shininess = shininess.Sample(samplerState_shininess, v.fUv);
    return ret;
}
)###";

namespace mana {
    GeometryPass::GeometryPass(RenderDevice &device) {
        shaderTextureNormals = device.createShaderProgram(SHADER_VERT_GEOMETRY,
                                                          SHADER_FRAG_GEOMETRY_TEXTURENORMALS,
                                                          Renderer3D::getShaderMacros(),
                                                          Renderer3D::getShaderIncludeCallback());
        shaderVertexNormals = device.createShaderProgram(SHADER_VERT_GEOMETRY,
                                                         SHADER_FRAG_GEOMETRY_VERTEXNORMALS,
                                                         Renderer3D::getShaderMacros(),
                                                         Renderer3D::getShaderIncludeCallback());
    }

    GeometryPass::~GeometryPass() {
        delete shaderTextureNormals;
        delete shaderVertexNormals;
    }

    void GeometryPass::render(RenderTarget &screen, GeometryBuffer &gBuffer, RenderScene &scene) {
        auto &ren = gBuffer.getRenderDevice().getRenderer();

        shaderTextureNormals->setTexture("diffuse", 0);
        shaderTextureNormals->setTexture("ambient", 1);
        shaderTextureNormals->setTexture("specular", 2);
        shaderTextureNormals->setTexture("shininess", 3);
        shaderTextureNormals->setTexture("emissive", 4);
        shaderTextureNormals->setTexture("normal", 5);

        shaderVertexNormals->setTexture("diffuse", 0);
        shaderVertexNormals->setTexture("ambient", 1);
        shaderVertexNormals->setTexture("specular", 2);
        shaderVertexNormals->setTexture("shininess", 3);
        shaderVertexNormals->setTexture("emissive", 4);

        //Clear geometry buffer
        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getRenderTarget().getSize()));

        // Rasterize the geometry and store the geometry + shading data in the geometry buffer.
        for (auto &command : scene.deferred) {
            assert(command.meshBuffer != nullptr);
            assert(command.material.diffuseTexture != nullptr);
            assert(command.material.ambientTexture != nullptr);
            assert(command.material.specularTexture != nullptr);
            assert(command.material.shininessTexture != nullptr);
            assert(command.material.emissiveTexture != nullptr);

            RenderCommand c;
            c.meshBuffers.emplace_back(command.meshBuffer);
            c.textures.emplace_back(command.material.diffuseTexture);
            c.textures.emplace_back(command.material.ambientTexture);
            c.textures.emplace_back(command.material.specularTexture);
            c.textures.emplace_back(command.material.shininessTexture);
            c.textures.emplace_back(command.material.emissiveTexture);

            if (command.material.normalTexture == nullptr) {
                c.shader = shaderVertexNormals;
            } else {
                c.shader = shaderTextureNormals;
                c.textures.emplace_back(command.material.normalTexture);
            }

            ren.addCommand(c);
        }

        ren.renderFinish();
    }
}