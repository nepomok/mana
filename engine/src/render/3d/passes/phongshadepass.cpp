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

#include "engine/render/3d/passes/phongshadepass.hpp"

#include "engine/render/3d/renderer3d.hpp"

#include "engine/math/rotation.hpp"

#include "engine/render/shadercompiler.hpp"

const char *SHADER_VERT_LIGHTING = R"###(
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

const char *SHADER_FRAG_LIGHTING = R"###(
#include "mana.hlsl"

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 phong_ambient: SV_TARGET0;
    float4 phong_diffuse: SV_TARGET1;
    float4 phong_specular: SV_TARGET2;
    float4 phong_combined: SV_TARGET3;
};

Texture2DMS<float4> position;
Texture2DMS<float4> normal;
Texture2DMS<float4> diffuse;
Texture2DMS<float4> ambient;
Texture2DMS<float4> specular;
Texture2DMS<float4> shininess;
Texture2DMS<float4> depth;

LightComponents getSampleLightComponents(int2 texCoord, int sample)
{
    float3 fragPosition = position.Load(texCoord, sample).xyz;
    float3 fragNormal = normal.Load(texCoord, sample).xyz;
    float4 fragDiffuse = diffuse.Load(texCoord, sample);
    float4 fragSpecular = specular.Load(texCoord, sample);
    float fragShininess = shininess.Load(texCoord, sample).r;

    return mana_calculate_light(fragPosition,
                                fragNormal,
                                fragDiffuse,
                                fragSpecular,
                                fragShininess);
}

LightComponents getAveragedLightComponents(int2 coord, int samples)
{
    LightComponents ret;

    for (int i = 0; i < samples; i++)
    {
        //Calculate lighting per sample to avoid averaging values such as position or normals
        LightComponents sampleComp = getSampleLightComponents(coord, i);

        ret.ambient += sampleComp.ambient;
        ret.diffuse += sampleComp.diffuse;
        ret.specular += sampleComp.specular;
    }

    // Dividing the resulting phong colors by the number of samples mathematically is correct and returns the averaged
    // phong colors, however for some reason the color is brighter than with only one sample and becomes
    // brighter with more samples.
    ret.ambient /= samples;
    ret.diffuse /= samples;
    ret.specular /= samples;

    return ret;
}

//Returns a float between 0 and 1 indicating how many samples are covered for the given fragment coordinates
float getSampleCoverage(int2 coord, int samples)
{
    int coveredSamples = 0;
    for (int i = 0; i < samples; i++)
    {
        float sampleDepth = depth.Load(coord, i);
        if (sampleDepth < 1)
        {
            coveredSamples++;
        }
    }
    float ret = coveredSamples;
    return ret / samples;
}

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;

    uint2 size;
    int samples;
    position.GetDimensions(size.x, size.y, samples);

    int2 coord = v.fUv * size;

    LightComponents comp = getAveragedLightComponents(coord, samples);

    //Use coverage value as alpha
    float coverage = getSampleCoverage(coord, samples);

    ret.phong_ambient = float4(comp.ambient, coverage);
    ret.phong_diffuse = float4(comp.diffuse, coverage);
    ret.phong_specular = float4(comp.specular, coverage);

    ret.phong_combined = ret.phong_ambient + ret.phong_diffuse + ret.phong_specular;
    ret.phong_combined.a = coverage;

    return ret;
}
)###";

namespace engine {
    using namespace ShaderCompiler;

    const char *PhongShadePass::AMBIENT = "phong_ambient";
    const char *PhongShadePass::DIFFUSE = "phong_diffuse";
    const char *PhongShadePass::SPECULAR = "phong_specular";
    const char *PhongShadePass::COMBINED = "phong_combined";

    PhongShadePass::PhongShadePass(RenderDevice &device)
            : renderDevice(device) {
        ShaderSource vertexShader(SHADER_VERT_LIGHTING,
                                  "main",
                                  VERTEX,
                                  HLSL_SHADER_MODEL_4);
        ShaderSource fragmentShader(SHADER_FRAG_LIGHTING,
                                    "main",
                                    FRAGMENT,
                                    HLSL_SHADER_MODEL_4);

        vertexShader.preprocess(Renderer3D::getShaderIncludeCallback(),
                                Renderer3D::getShaderMacros(HLSL_SHADER_MODEL_4));
        fragmentShader.preprocess(Renderer3D::getShaderIncludeCallback(),
                                  Renderer3D::getShaderMacros(HLSL_SHADER_MODEL_4));

        auto &allocator = device.getAllocator();

        shader = allocator.createShaderProgram(vertexShader, fragmentShader);
    }

    void PhongShadePass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer(AMBIENT, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(DIFFUSE, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(SPECULAR, TextureBuffer::ColorFormat::RGBA);
        gBuffer.addBuffer(COMBINED, TextureBuffer::ColorFormat::RGBA);
    }

    void PhongShadePass::render(GeometryBuffer &gBuffer, Scene &scene) {
        int dirCount = 0;
        int pointCount = 0;
        int spotCount = 0;

        shader->activate();

        for (auto &light: scene.lights) {
            std::string name;
            switch (light.type) {
                case LIGHT_DIRECTIONAL:
                    name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(dirCount++) + "].";
                    shader->setVec3(name + "direction", light.direction);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
                case LIGHT_POINT:
                    name = "MANA_LIGHTS_POINT[" + std::to_string(pointCount++) + "].";
                    shader->setVec3(name + "position", light.transform.getPosition());
                    shader->setFloat(name + "constantValue", light.constant);
                    shader->setFloat(name + "linearValue", light.linear);
                    shader->setFloat(name + "quadraticValue", light.quadratic);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
                case LIGHT_SPOT:
                    name = "MANA_LIGHTS_SPOT[" + std::to_string(spotCount++) + "].";
                    shader->setVec3(name + "position", light.transform.getPosition());
                    shader->setVec3(name + "direction", light.direction);
                    shader->setFloat(name + "cutOff", cosf(degreesToRadians(light.cutOff)));
                    shader->setFloat(name + "outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                    shader->setFloat(name + "constantValue", light.constant);
                    shader->setFloat(name + "linearValue", light.linear);
                    shader->setFloat(name + "quadraticValue", light.quadratic);
                    shader->setVec3(name + "ambient", light.ambient);
                    shader->setVec3(name + "diffuse", light.diffuse);
                    shader->setVec3(name + "specular", light.specular);
                    break;
            }
        }

        shader->setInt("MANA_LIGHT_COUNT_DIRECTIONAL", dirCount);
        shader->setInt("MANA_LIGHT_COUNT_POINT", pointCount);
        shader->setInt("MANA_LIGHT_COUNT_SPOT", spotCount);

        shader->setVec3("MANA_VIEWPOS", scene.camera.transform.getPosition());

        shader->setTexture("position", 0);
        shader->setTexture("normal", 1);
        shader->setTexture("diffuse", 2);
        shader->setTexture("ambient", 3);
        shader->setTexture("specular", 4);
        shader->setTexture("shininess", 5);
        shader->setTexture("depth", 6);

        RenderCommand command(*shader, gBuffer.getScreenQuad());

        command.textures.emplace_back(gBuffer.getBuffer("position"));
        command.textures.emplace_back(gBuffer.getBuffer("normal"));
        command.textures.emplace_back(gBuffer.getBuffer("diffuse"));
        command.textures.emplace_back(gBuffer.getBuffer("ambient"));
        command.textures.emplace_back(gBuffer.getBuffer("specular"));
        command.textures.emplace_back(gBuffer.getBuffer("shininess"));
        command.textures.emplace_back(gBuffer.getBuffer("depth"));

        command.properties.enableDepthTest = false;
        command.properties.enableStencilTest = false;
        command.properties.enableFaceCulling = false;
        command.properties.enableBlending = true;

        auto &ren = renderDevice.getRenderer();

        gBuffer.attachColor({"phong_ambient", "phong_diffuse", "phong_specular", "phong_combined"});
        gBuffer.detachDepthStencil();

        ren.renderBegin(gBuffer.getRenderTarget(), RenderOptions({}, gBuffer.getSize(), true, {255, 0, 0, 0}));
        ren.addCommand(command);
        ren.renderFinish();
    }
}