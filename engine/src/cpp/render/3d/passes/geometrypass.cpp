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

#include <sstream>

#include "engine/render/3d/passes/geometrypass.hpp"

#include "engine/render/3d/renderer3d.hpp"

#include "engine/asset/assetloader.hpp"

static const char *SHADER_VERT_GEOMETRY = R"###(
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

static const char *SHADER_FRAG_GEOMETRY_TEXTURENORMALS = R"###(
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
     float4 lighting     :   SV_TARGET5;
     float4 id           :   SV_TARGET6;
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
    ret.lighting.r = shininess.Sample(samplerState_shininess, v.fUv).r;
    ret.lighting.g = 1.0f;
    ret.id = 1;
    return ret;
}
)###";

static const char *SHADER_FRAG_GEOMETRY_VERTEXNORMALS = R"###(
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
     float4 lighting     :   SV_TARGET5;
     float4 id           :   SV_TARGET6;
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
    ret.lighting.r = shininess.Sample(samplerState_shininess, v.fUv).r;
    ret.lighting.g = 1.0f;
    ret.id = 1;
    return ret;
}
)###";

static const char *SHADER_VERT_SKYBOX = R"###(
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
    float3  fPos : POSITION0;
    float3  fNorm : NORMAL;
    float2  fUv : TEXCOORD0;
    float4 vPos : SV_Position;
    float3 worldPos : POSITION1;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;

    float4x4 t = mul(MANA_VIEW_TRANSLATION, mul(MANA_V, MANA_P));

    ret.vPos = mul(float4(v.position, 1), t);
    ret.fPos = mul(float4(v.position, 1), t).xyz;
    ret.worldPos = v.position;
    ret.fNorm = mul(float4(v.normal, 1), transpose(MANA_M_INVERT)).xyz;
    ret.fUv = v.uv;

    return ret;
}
)###";

static const char *SHADER_FRAG_SKYBOX = R"###(
struct PS_INPUT {
    float3 fPos: POSITION0;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
    float3 worldPos : POSITION1;
};

struct PS_OUTPUT {
     float4 position     :   SV_TARGET0;
     float4 normal       :   SV_TARGET1;
     float4 diffuse      :   SV_TARGET2;
     float4 ambient      :   SV_TARGET3;
     float4 specular     :   SV_TARGET4;
     float4 lighting     :   SV_TARGET5;
     float4 id           :   SV_TARGET6;
};

TextureCube diffuse;

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.position = float4(v.fPos, 1);
    ret.normal = float4(v.fNorm, 1);
    ret.diffuse = diffuse.Sample(samplerState_diffuse, v.worldPos);
    ret.ambient = float4(0,0,0,0);
    ret.specular = float4(0,0,0,0);
    ret.lighting.r = 0.0f;
    ret.lighting.g = 0.0f;
    ret.id = 0;
    return ret;
}
)###";

static const std::string SKYBOX_OBJ = std::string(R"###(
o Cube
v 1.000000 1.000000 -1.000000
v 1.000000 -1.000000 -1.000000
v 1.000000 1.000000 1.000000
v 1.000000 -1.000000 1.000000
v -1.000000 1.000000 -1.000000
v -1.000000 -1.000000 -1.000000
v -1.000000 1.000000 1.000000
v -1.000000 -1.000000 1.000000
vt 0.000000 1.000000
vt 1.000000 0.000000
vt 1.000000 1.000000
vt 1.000000 1.000000
vt 0.000000 0.000000
vt 1.000000 0.000000
vt 0.000000 1.000000
vt 1.000000 0.000000
vt 1.000000 1.000000
vt 0.000000 1.000000
vt 0.000000 0.000000
vt 1.000000 0.000000
vt 0.000000 0.000000
vt 0.000000 0.000000
vt 1.000000 1.000000
vt 0.000000 1.000000
vn 0.0000 1.0000 0.0000
vn 0.0000 0.0000 1.0000
vn -1.0000 0.0000 0.0000
vn 0.0000 -1.0000 0.0000
vn 1.0000 0.0000 0.0000
vn 0.0000 0.0000 -1.0000
s off
f 5/1/1 3/2/1 1/3/1
f 3/4/2 8/5/2 4/6/2
f 7/7/3 6/8/3 8/5/3
f 2/9/4 8/5/4 6/10/4
f 1/3/5 4/11/5 2/12/5
f 5/1/6 2/12/6 6/13/6
f 5/1/1 7/14/1 3/2/1
f 3/4/2 7/7/2 8/5/2
f 7/7/3 5/15/3 6/8/3
f 2/9/4 4/6/4 8/5/4
f 1/3/5 3/16/5 4/11/5
f 5/1/6 1/3/6 2/12/6
)###");

namespace mana {
    GeometryPass::GeometryPass(RenderDevice &device)
            : renderDevice(device) {
        auto &allocator = device.getAllocator();

        shaderTextureNormals = allocator.createShaderProgram(SHADER_VERT_GEOMETRY,
                                                             SHADER_FRAG_GEOMETRY_TEXTURENORMALS,
                                                             Renderer3D::getShaderMacros(),
                                                             Renderer3D::getShaderIncludeCallback());
        shaderVertexNormals = allocator.createShaderProgram(SHADER_VERT_GEOMETRY,
                                                            SHADER_FRAG_GEOMETRY_VERTEXNORMALS,
                                                            Renderer3D::getShaderMacros(),
                                                            Renderer3D::getShaderIncludeCallback());
        TextureBuffer::Attributes attributes;
        attributes.size = Vec2i(1, 1);
        attributes.format = TextureBuffer::RGBA;
        attributes.textureType = TextureBuffer::TEXTURE_2D;
        attributes.generateMipmap = false;
        attributes.wrapping = TextureBuffer::REPEAT;

        diffuseDefault = allocator.createTextureBuffer(attributes);
        ambientDefault = allocator.createTextureBuffer(attributes);
        specularDefault = allocator.createTextureBuffer(attributes);
        emissiveDefault = allocator.createTextureBuffer(attributes);

        attributes.format = TextureBuffer::R32F;
        shininessDefault = allocator.createTextureBuffer(attributes);

        attributes.format = TextureBuffer::RGBA;
        attributes.textureType = TextureBuffer::TEXTURE_CUBE_MAP;
        skyboxDefault = allocator.createTextureBuffer(attributes);

        shaderSkybox = allocator.createShaderProgram(SHADER_VERT_SKYBOX,
                                                     SHADER_FRAG_SKYBOX,
                                                     Renderer3D::getShaderMacros(),
                                                     Renderer3D::getShaderIncludeCallback());
        shaderSkybox->setTexture("diffuse", 0);

        std::stringstream skyboxStream((std::string(SKYBOX_OBJ)));

        Archive archive;
        Mesh skyboxMesh = AssetLoader::loadBundle(skyboxStream, ".obj", archive, ThreadPool::pool).meshes.at("Cube");
        skyboxCube = allocator.createMeshBuffer(skyboxMesh);
    }

    GeometryPass::~GeometryPass() {
        delete shaderTextureNormals;
        delete shaderVertexNormals;
        delete diffuseDefault;
        delete ambientDefault;
        delete specularDefault;
        delete emissiveDefault;
        delete shininessDefault;
        delete shaderSkybox;
        delete skyboxCube;
    }

    void GeometryPass::render(RenderTarget &screen, RenderScene &scene, GeometryBuffer &gBuffer) {
        auto &ren = renderDevice.getRenderer();

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

        //Draw skybox
        {
            RenderCommand command;
            command.meshBuffers.emplace_back(skyboxCube);

            if (scene.skybox == nullptr) {
                for (int i = TextureBuffer::CubeMapFace::POSITIVE_X; i <= TextureBuffer::CubeMapFace::NEGATIVE_Z; i++) {
                    skyboxDefault->upload(static_cast<TextureBuffer::CubeMapFace>(i),
                                          Image<ColorRGBA>(1, 1, {scene.skyboxColor}));
                }
                command.textures.emplace_back(skyboxDefault);
            } else {
                command.textures.emplace_back(scene.skybox);
            }

            command.properties.enableDepthTest = false;
            command.properties.enableFaceCulling = false;

            command.shader = shaderSkybox;

            command.shader->setMat4("MANA_M", model);
            command.shader->setMat4("MANA_V", view);
            command.shader->setMat4("MANA_P", projection);
            command.shader->setMat4("MANA_MVP", projection * view * model);
            command.shader->setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
            command.shader->setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

            ren.addCommand(command);
        }

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
                shininessDefault->upload(Image<float>(1, 1, {command.material.shininess}));
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