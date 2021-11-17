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

static const char *SHADER_VERT_GEOMETRY = R"###(#version 460
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUv;
layout (location = 3) in vec3 vTangent;
layout (location = 4) in vec3 vBitangent;
layout (location = 5) in vec4 vInstanceRow0;
layout (location = 6) in vec4 vInstanceRow1;
layout (location = 7) in vec4 vInstanceRow2;
layout (location = 8) in vec4 vInstanceRow3;

layout(location = 0) out vec3 fPos;
layout(location = 1) out vec3 fNorm;
layout(location = 2) out vec2 fUv;
layout(location = 4) out vec4 vPos;
layout(location = 5) out vec3 N;
layout(location = 6) out vec3 T;
layout(location = 7) out vec3 B;

layout(location = 0) uniform mat4 MANA_M;
layout(location = 1) uniform mat4 MANA_MVP;

layout(location = 2) uniform int hasTextureNormal;

layout(location = 3) uniform vec4 diffuseColor;
layout(location = 4) uniform vec4 ambientColor;
layout(location = 5) uniform vec4 specularColor;
layout(location = 6) uniform float shininessColor;

layout(location = 7) uniform vec4 emissiveColor;

layout(location = 8) uniform sampler2D diffuse;
layout(location = 9) uniform sampler2D ambient;
layout(location = 10) uniform sampler2D specular;
layout(location = 11) uniform sampler2D shininess;
layout(location = 12) uniform sampler2D emissive;

layout(location = 13) uniform sampler2D normal;

void main()
{
    mat4 instanceMatrix = mat4(vInstanceRow0, vInstanceRow1, vInstanceRow2, vInstanceRow3);

    vPos = (instanceMatrix * MANA_MVP) * vec4(vPosition, 1);
    fPos = ((instanceMatrix * MANA_M) * vec4(vPosition, 1)).xyz;
    fUv = vUv;

    // Transform the normal, tangent and bitangent vectors into world space by multiplying with the model and instance matrix.

    // The model matrix is multiplied with the instance matrix so that the model transformation + instance transformation are applied to the normals.
    mat4 normalMatrix = instanceMatrix * MANA_M;

    fNorm = normalize((normalMatrix * vec4(vNormal, 1)).xyz);

    T = normalize((normalMatrix * vec4(vTangent, 1)).xyz);
    B = normalize((normalMatrix * vec4(vBitangent, 1)).xyz);
    N = normalize((normalMatrix * vec4(vNormal, 1)).xyz);

    gl_Position = vPos;
}
)###";

static const char *SHADER_FRAG_GEOMETRY = R"###(#version 460
layout(location = 0) in vec3 fPos;
layout(location = 1) in vec3 fNorm;
layout(location = 2) in vec2 fUv;
layout(location = 4) in vec4 vPos;
layout(location = 5) in vec3 N;
layout(location = 6) in vec3 T;
layout(location = 7) in vec3 B;

layout(location = 0) out vec4 oPosition;
layout(location = 1) out vec4 oNormal;
layout(location = 2) out vec4 oDiffuse;
layout(location = 3) out vec4 oAmbient;
layout(location = 4) out vec4 oSpecular;
layout(location = 5) out vec4 oShininess;
layout(location = 6) out vec4 oEmissive;
layout(location = 7) out vec4 oId;

layout(location = 0) uniform mat4 MANA_M;
layout(location = 1) uniform mat4 MANA_MVP;

layout(location = 2) uniform int hasTextureNormal;

layout(location = 3) uniform vec4 diffuseColor;
layout(location = 4) uniform vec4 ambientColor;
layout(location = 5) uniform vec4 specularColor;
layout(location = 6) uniform float shininessColor;

layout(location = 7) uniform vec4 emissiveColor;

layout(location = 8) uniform sampler2D diffuse;
layout(location = 9) uniform sampler2D ambient;
layout(location = 10) uniform sampler2D specular;
layout(location = 11) uniform sampler2D shininess;
layout(location = 12) uniform sampler2D emissive;

layout(location = 13) uniform sampler2D normal;

void main() {
    oPosition = vec4(fPos, 1);

    if (hasTextureNormal != 0)
    {
        // Combine the 3 interpolated tangent, bitangent and normals into the TBN matrix
        mat3 TBN = mat3(T, B, N);

        // Sample tangent space normal from texture, texture is assigned correctly before the shader is invoked.
        vec3 tangentNormal = texture(normal, fUv).xyz;

        // Scale tangent space normal into -1 / 1 range
        tangentNormal = normalize((tangentNormal * 2) - 1);

        // Transform the tangent space normal into world space by multiplying with the TBN matrix.
        vec3 norm = TBN * tangentNormal;

        // Assign the world space normal
        // The value assigned here is not the correct normal.
        oNormal = vec4(norm, 1);
    }
    else
    {
        oNormal = vec4(fNorm, 1);
    }

    oDiffuse = texture(diffuse, fUv) + diffuseColor;
    oAmbient = texture(ambient, fUv) + ambientColor;
    oSpecular = texture(specular, fUv) + specularColor;
    oShininess.r = texture(shininess, fUv).r + shininessColor;

    oId.x = 1;
}
)###";

namespace engine {
    using namespace ShaderCompiler;

    static inline Vec4f scaleColor(const ColorRGBA &color) {
        return {static_cast<float>(color.r()) * 255,
                static_cast<float>(color.g()) * 255,
                static_cast<float>(color.b()) * 255,
                static_cast<float>(color.a()) * 255};
    }

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
        ShaderSource vs(SHADER_VERT_GEOMETRY, "main", VERTEX, GLSL_460);
        ShaderSource fs(SHADER_FRAG_GEOMETRY, "main", FRAGMENT, GLSL_460);

        vs.preprocess(Renderer3D::getShaderIncludeCallback(),
                      Renderer3D::getShaderMacros(GLSL_460));
        fs.preprocess(Renderer3D::getShaderIncludeCallback(),
                      Renderer3D::getShaderMacros(GLSL_460));

        auto &allocator = device.getAllocator();
        shader = allocator.createShaderProgram(vs, fs);

        TextureBuffer::Attributes attributes;
        attributes.size = {1, 1};
        defaultTexture = allocator.createTextureBuffer(attributes);
        defaultTexture->upload(Image<ColorRGBA>(1, 1, {{0, 0, 0, 0}}));
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

        Mat4f model, view, projection;
        view = scene.camera.view();
        projection = scene.camera.projection();

        shader->activate();

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

        std::vector<std::reference_wrapper<TextureBuffer>> textures;

        // Rasterize the geometry and store the geometry + shading data in the geometry buffer.
        for (auto &command: scene.deferred) {
            textures.clear();

            int textureIndex = 0;

            if (command.material.diffuseTexture == nullptr) {
                shader->setVec4(3, scaleColor(command.material.diffuse));
                shader->setTexture(8, textureIndex++);
                textures.emplace_back(*defaultTexture);
            } else {
                shader->setVec4(3, scaleColor({0, 0, 0, 0}));
                shader->setTexture(8, textureIndex++);
                textures.emplace_back(*command.material.diffuseTexture);
            }

            if (command.material.ambientTexture == nullptr) {
                shader->setVec4(4, scaleColor(command.material.ambient));
                shader->setTexture(9, textureIndex++);
                textures.emplace_back(*defaultTexture);
            } else {
                shader->setVec4(4, scaleColor({0, 0, 0, 0}));
                shader->setTexture(9, textureIndex++);
                textures.emplace_back(*command.material.ambientTexture);
            }

            if (command.material.specularTexture == nullptr) {
                shader->setVec4(5, scaleColor(command.material.specular));
                shader->setTexture(10, textureIndex++);
                textures.emplace_back(*defaultTexture);
            } else {
                shader->setVec4(5, scaleColor({0, 0, 0, 0}));
                shader->setTexture(10, textureIndex++);
                textures.emplace_back(*command.material.specularTexture);
            }

            if (command.material.shininessTexture == nullptr) {
                shader->setFloat(6, command.material.shininess);
                shader->setTexture(11, textureIndex++);
                textures.emplace_back(*defaultTexture);
            } else {
                shader->setVec4(6, scaleColor({0, 0, 0, 0}));
                shader->setTexture(11, textureIndex++);
                textures.emplace_back(*command.material.shininessTexture);
            }

            if (command.material.emissiveTexture == nullptr) {
                shader->setVec4(7, scaleColor(command.material.emissive));
                shader->setTexture(12, textureIndex++);
                textures.emplace_back(*defaultTexture);
            } else {
                shader->setVec4(7, scaleColor({0, 0, 0, 0}));
                shader->setTexture(12, textureIndex++);
                textures.emplace_back(*command.material.emissiveTexture);
            }

            shader->setInt(2, command.material.normalTexture != nullptr);
            if (command.material.normalTexture != nullptr) {
                shader->setTexture(13, textureIndex++);
                textures.emplace_back(*command.material.normalTexture);
            }

            model = command.transform.model();

            shader->setMat4(0, model);
            shader->setMat4(1, projection * view * model);

            RenderCommand c(*shader);
            c.meshBuffers.emplace_back(command.meshBuffer);
            c.textures = textures;
            ren.addCommand(c);
        }

        ren.renderFinish();
    }
}