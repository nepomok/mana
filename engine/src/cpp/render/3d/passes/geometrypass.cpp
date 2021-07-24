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
        TextureBuffer::Attributes attributes;
        attributes.size = Vec2i(1, 1);
        attributes.format = TextureBuffer::RGBA;
        attributes.textureType = TextureBuffer::TEXTURE_2D;
        attributes.generateMipmap = false;
        attributes.wrapping = TextureBuffer::REPEAT;

        diffuseDefault = device.createTextureBuffer(attributes);
        ambientDefault = device.createTextureBuffer(attributes);
        specularDefault = device.createTextureBuffer(attributes);
        emissiveDefault = device.createTextureBuffer(attributes);

        attributes.format = TextureBuffer::R32F;
        shininessDefault = device.createTextureBuffer(attributes);
    }

    GeometryPass::~GeometryPass() {
        delete shaderTextureNormals;
        delete shaderVertexNormals;
        delete diffuseDefault;
        delete ambientDefault;
        delete specularDefault;
        delete emissiveDefault;
        delete shininessDefault;
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

        Mat4f model, view, projection, cameraTranslation;
        view = scene.camera.view();
        projection = scene.camera.projection();
        cameraTranslation = MatrixMath::translate(scene.camera.transform.position);

        // Rasterize the geometry and store the geometry + shading data in the geometry buffer.
        for (auto &command : scene.deferred) {
            assert(command.meshBuffer != nullptr);

            RenderCommand c;
            c.meshBuffers.emplace_back(command.meshBuffer);

            if (command.material.diffuseTexture == nullptr) {
                diffuseDefault->upload({1, 1, {command.material.diffuse}});
                c.textures.emplace_back(diffuseDefault);
            } else {
                c.textures.emplace_back(command.material.diffuseTexture);
            }
            if (command.material.ambientTexture == nullptr) {
                ambientDefault->upload({1, 1, {command.material.ambient}});
                c.textures.emplace_back(ambientDefault);
            } else {
                c.textures.emplace_back(command.material.ambientTexture);
            }
            if (command.material.specularTexture == nullptr) {
                specularDefault->upload({1, 1, {command.material.specular}});
                c.textures.emplace_back(specularDefault);
            } else {
                c.textures.emplace_back(command.material.specularTexture);
            }
            if (command.material.shininessTexture == nullptr) {
                shininessDefault->upload(ImageBuffer<float>(1, 1, {command.material.shininess}));
                c.textures.emplace_back(shininessDefault);
            } else {
                c.textures.emplace_back(command.material.shininessTexture);
            }
            if (command.material.emissiveTexture == nullptr) {
                emissiveDefault->upload({1, 1, {command.material.emissive}});
                c.textures.emplace_back(emissiveDefault);
            } else {
                c.textures.emplace_back(command.material.emissiveTexture);
            }

            if (command.material.normalTexture == nullptr) {
                c.shader = shaderVertexNormals;
            } else {
                c.shader = shaderTextureNormals;
                c.textures.emplace_back(command.material.normalTexture);
            }

            model = MatrixMath::translate(command.transform.position);
            model = model * MatrixMath::scale(command.transform.scale);
            model = model * MatrixMath::rotate(command.transform.rotation);

            c.shader->setMat4("MANA_M", model);
            c.shader->setMat4("MANA_V", view);
            c.shader->setMat4("MANA_P", projection);
            c.shader->setMat4("MANA_MVP", projection * view * model);
            c.shader->setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
            c.shader->setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

            ren.addCommand(c);
        }

        ren.renderFinish();
    }
}