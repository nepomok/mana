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

#include "engine/asset/assetimporter.hpp"

static const char *SHADER_VERT_GEOMETRY = R"###(
#include "mana.hlsl"

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
    float3 N: TEXCOORD1;
    float3 T: TEXCOORD2;
    float3 B: TEXCOORD3;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;

    float4x4 instanceMatrix = float4x4(v.instanceRow0, v.instanceRow1, v.instanceRow2, v.instanceRow3);

    ret.vPos = mul(float4(v.position, 1), mul(instanceMatrix, MANA_MVP));
    ret.fPos = mul(float4(v.position, 1), mul(instanceMatrix, MANA_M)).xyz;
    ret.fUv = v.uv;

    // Transform the normal, tangent and bitangent vectors into world space by multiplying with the model and instance matrix.

    // The model matrix is multiplied with the instance matrix so that the model transformation + instance transformation are applied to the normals.
    float4x4 normalMatrix = mul(instanceMatrix, MANA_M);

    ret.fNorm = normalize(mul(float4(v.normal, 0), normalMatrix).xyz);

    ret.T = normalize(mul(float4(v.tangent, 0), normalMatrix).xyz);
    ret.B = normalize(mul(float4(v.bitangent, 0), normalMatrix).xyz);
    ret.N = normalize(mul(float4(v.normal, 0), normalMatrix).xyz);

    return ret;
}
)###";

static const char *SHADER_FRAG_GEOMETRY_TEXTURENORMALS = R"###(
struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
    float3 N: TEXCOORD1;
    float3 T: TEXCOORD2;
    float3 B: TEXCOORD3;
};

struct PS_OUTPUT {
     float4 position     :   SV_TARGET0;
     float4 normal       :   SV_TARGET1;
     float4 diffuse      :   SV_TARGET2;
     float4 ambient      :   SV_TARGET3;
     float4 specular     :   SV_TARGET4;
     float4 shininess    :   SV_TARGET5;
     float4 id           :   SV_TARGET6;
};

Texture2D diffuse;
Texture2D ambient;
Texture2D specular;
Texture2D shininess;
Texture2D emissive;
Texture2D normal;

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
SamplerState samplerState_normal
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.position = float4(v.fPos, 1);

    // Combine the 3 interpolated tangent, bitangent and normals into the TBN matrix
    float3x3 TBN = float3x3(v.T, v.B, v.N);

    // Sample tangent space normal from texture, texture is assigned correctly before the shader is invoked.
    float4 tangentNormal = normal.Sample(samplerState_normal, v.fUv);

    // Scale tangent space normal into -1 / 1 range
    tangentNormal = (tangentNormal * 2) - 1;

    // Transform the tangent space normal into world space by multiplying with the TBN matrix.
    float3 norm = mul(tangentNormal, TBN);

    // Assign the world space normal
    // The value assigned here is not the correct normal.
    ret.normal = float4(norm, 1);

    ret.diffuse = diffuse.Sample(samplerState_diffuse, v.fUv);
    ret.ambient = ambient.Sample(samplerState_ambient, v.fUv);
    ret.specular = specular.Sample(samplerState_specular, v.fUv);
    ret.shininess.r = shininess.Sample(samplerState_shininess, v.fUv).r;
    ret.id.x = 1;
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
     float4 shininess     :   SV_TARGET5;
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
    ret.shininess.r = shininess.Sample(samplerState_shininess, v.fUv).r;
    ret.id.x = 1;
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
    float3 tangent: TANGENT;
    float3 bitangent: BINORMAL;
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
    ret.fNorm = mul(v.normal, MANA_M).xyz;
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
     float4 color     :   SV_TARGET0;
};

TextureCube diffuse;

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.color = diffuse.Sample(samplerState_diffuse, v.worldPos);
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

namespace engine {
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

        std::stringstream skyboxStream((std::string(SKYBOX_OBJ)));
        Mesh skyboxMesh = AssetImporter::import(skyboxStream, ".obj").meshes.at("Cube");
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

    void GeometryPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer("depth", TextureBuffer::ColorFormat::DEPTH_STENCIL);
        gBuffer.addBuffer("position", TextureBuffer::ColorFormat::RGBA32F);
        gBuffer.addBuffer("normal", TextureBuffer::ColorFormat::RGBA32F);
        gBuffer.addBuffer("diffuse", TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer("ambient", TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer("specular", TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer("shininess", TextureBuffer::ColorFormat::R32F);
        gBuffer.addBuffer("id", TextureBuffer::ColorFormat::R8UI);
        gBuffer.addBuffer("skybox", TextureBuffer::ColorFormat::RGBA);
    }

    void GeometryPass::render(GeometryBuffer &gBuffer, RenderScene &scene) {
        auto &ren = renderDevice.getRenderer();

        //Set shader texture attachment points
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

        shaderSkybox->setTexture("diffuse", 0);

        Mat4f model, view, projection, cameraTranslation;
        view = scene.camera.view();
        projection = scene.camera.projection();
        cameraTranslation = MatrixMath::translate(scene.camera.transform.position);

        //Draw skybox
        gBuffer.attachColor({"skybox"});
        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getRenderTarget().getSize()));

        RenderCommand skyboxCommand;
        skyboxCommand.meshBuffers.emplace_back(skyboxCube);

        if (scene.skybox == nullptr) {
            for (int i = TextureBuffer::CubeMapFace::POSITIVE_X; i <= TextureBuffer::CubeMapFace::NEGATIVE_Z; i++) {
                skyboxDefault->upload(static_cast<TextureBuffer::CubeMapFace>(i),
                                      Image<ColorRGBA>(1, 1, {scene.skyboxColor}));
            }
            skyboxCommand.textures.emplace_back(skyboxDefault);
        } else {
            skyboxCommand.textures.emplace_back(scene.skybox);
        }

        skyboxCommand.properties.enableDepthTest = false;
        skyboxCommand.properties.enableFaceCulling = false;

        skyboxCommand.shader = shaderSkybox;

        skyboxCommand.shader->setMat4("MANA_M", model);
        skyboxCommand.shader->setMat4("MANA_V", view);
        skyboxCommand.shader->setMat4("MANA_P", projection);
        skyboxCommand.shader->setMat4("MANA_MVP", projection * view * model);
        skyboxCommand.shader->setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
        skyboxCommand.shader->setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

        ren.addCommand(skyboxCommand);

        ren.renderFinish();

        // Draw deferred geometry
        gBuffer.attachDepthStencil("depth");
        gBuffer.attachColor({
                                    "position",
                                    "normal",
                                    "diffuse",
                                    "ambient",
                                    "specular",
                                    "shininess",
                                    "id"
                            });

        //Clear geometry buffer
        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getRenderTarget().getSize()));

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

        gBuffer.detachDepthStencil();
    }
}