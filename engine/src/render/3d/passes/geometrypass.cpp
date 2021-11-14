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
#include "engine/render/shadercompiler.hpp"

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

    ret.fNorm = normalize(mul(float4(v.normal, 1), normalMatrix).xyz);

    ret.T = normalize(mul(float4(v.tangent, 1), normalMatrix).xyz);
    ret.B = normalize(mul(float4(v.bitangent, 1), normalMatrix).xyz);
    ret.N = normalize(mul(float4(v.normal, 1), normalMatrix).xyz);

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
     float4 emissive     :   SV_TARGET6;
     float4 id           :   SV_TARGET7;
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
    float3 tangentNormal = normal.Sample(samplerState_normal, v.fUv);

    // Scale tangent space normal into -1 / 1 range
    tangentNormal = normalize((tangentNormal * 2) - 1);

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
     float4 shininess    :   SV_TARGET5;
     float4 emissive     :   SV_TARGET6;
     float4 id           :   SV_TARGET7;
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

namespace engine {
    using namespace ShaderCompiler;

    const char *GeometryPass::DEPTH = "depth";
    const char *GeometryPass::POSITION = "position";
    const char *GeometryPass::NORMAL = "normal";
    const char *GeometryPass::DIFFUSE = "diffuse";
    const char *GeometryPass::AMBIENT = "ambient";
    const char *GeometryPass::SPECULAR = "specular";
    const char *GeometryPass::SHININESS = "shininess";
    const char *GeometryPass::EMISSIVE = "emissive";
    const char *GeometryPass::ID = "id";

    GeometryPass::GeometryPass(RenderDevice &device)
            : renderDevice(device) {
        ShaderSource vs(SHADER_VERT_GEOMETRY, "main", VERTEX, HLSL_SHADER_MODEL_4);
        ShaderSource fsVertNorm(SHADER_FRAG_GEOMETRY_VERTEXNORMALS, "main", FRAGMENT, HLSL_SHADER_MODEL_4);
        ShaderSource fsTexNorm(SHADER_FRAG_GEOMETRY_TEXTURENORMALS, "main", FRAGMENT, HLSL_SHADER_MODEL_4);

        vs.preprocess(Renderer3D::getShaderIncludeCallback(),
                      Renderer3D::getShaderMacros(HLSL_SHADER_MODEL_4));
        fsVertNorm.preprocess(Renderer3D::getShaderIncludeCallback(),
                              Renderer3D::getShaderMacros(HLSL_SHADER_MODEL_4));
        fsTexNorm.preprocess(Renderer3D::getShaderIncludeCallback(),
                             Renderer3D::getShaderMacros(HLSL_SHADER_MODEL_4));

        auto &allocator = device.getAllocator();
        shaderVertexNormals = allocator.createShaderProgram(vs, fsVertNorm);
        shaderTextureNormals = allocator.createShaderProgram(vs, fsTexNorm);

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
    }

    GeometryPass::~GeometryPass() = default;

    void GeometryPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer(DEPTH, TextureBuffer::ColorFormat::DEPTH_STENCIL);
        gBuffer.addBuffer(POSITION, TextureBuffer::ColorFormat::RGBA32F);
        gBuffer.addBuffer(NORMAL, TextureBuffer::ColorFormat::RGBA32F);
        gBuffer.addBuffer(DIFFUSE, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(AMBIENT, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(SPECULAR, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(SHININESS, TextureBuffer::ColorFormat::R32F);
        gBuffer.addBuffer(EMISSIVE, TextureBuffer::ColorFormat::R32F);
        gBuffer.addBuffer(ID, TextureBuffer::ColorFormat::R8UI);
    }

    void GeometryPass::render(GeometryBuffer &gBuffer, Scene &scene) {
        auto &ren = renderDevice.getRenderer();

        Mat4f model, view, projection, cameraTranslation;
        view = scene.camera.view();
        projection = scene.camera.projection();
        cameraTranslation = MatrixMath::translate(scene.camera.transform.getPosition());

        shaderVertexNormals->setMat4("MANA_V", view);
        shaderVertexNormals->setMat4("MANA_P", projection);
        shaderVertexNormals->setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

        shaderTextureNormals->setMat4("MANA_V", view);
        shaderTextureNormals->setMat4("MANA_P", projection);
        shaderTextureNormals->setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

        // Draw deferred geometry
        gBuffer.attachColor({
                                    POSITION,
                                    NORMAL,
                                    DIFFUSE,
                                    AMBIENT,
                                    SPECULAR,
                                    SHININESS,
                                    EMISSIVE,
                                    ID
                            });
        gBuffer.attachDepthStencil(DEPTH);

        //Clear geometry buffer
        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getRenderTarget().getSize()));

        // Rasterize the geometry and store the geometry + shading data in the geometry buffer.
        for (auto &command: scene.deferred) {
            std::vector<std::reference_wrapper<TextureBuffer>> textures;

            if (command.material.diffuseTexture == nullptr) {
                diffuseDefault->upload({1, 1, {command.material.diffuse}});
                textures.emplace_back(*diffuseDefault);
            } else {
                textures.emplace_back(*command.material.diffuseTexture);
            }
            if (command.material.ambientTexture == nullptr) {
                ambientDefault->upload({1, 1, {command.material.ambient}});
                textures.emplace_back(*ambientDefault);
            } else {
                textures.emplace_back(*command.material.ambientTexture);
            }
            if (command.material.specularTexture == nullptr) {
                specularDefault->upload({1, 1, {command.material.specular}});
                textures.emplace_back(*specularDefault);
            } else {
                textures.emplace_back(*command.material.specularTexture);
            }
            if (command.material.shininessTexture == nullptr) {
                shininessDefault->upload(Image<float>(1, 1, {command.material.shininess}));
                textures.emplace_back(*shininessDefault);
            } else {
                textures.emplace_back(*command.material.shininessTexture);
            }
            if (command.material.emissiveTexture == nullptr) {
                emissiveDefault->upload({1, 1, {command.material.emissive}});
                textures.emplace_back(*emissiveDefault);
            } else {
                textures.emplace_back(*command.material.emissiveTexture);
            }

            ShaderProgram *shader;
            if (command.material.normalTexture == nullptr) {
                shader = shaderVertexNormals.get();
            } else {
                shader = shaderTextureNormals.get();
                textures.emplace_back(*command.material.normalTexture);
            }

            model = command.transform.model();

            shader->setMat4("MANA_M", model);
            shader->setMat4("MANA_MVP", projection * view * model);

            RenderCommand c(*shader);
            c.meshBuffers.emplace_back(command.meshBuffer);
            c.textures = textures;
            ren.addCommand(c);
        }

        ren.renderFinish();
    }
}