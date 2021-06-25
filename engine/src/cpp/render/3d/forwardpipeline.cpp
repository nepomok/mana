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

#include "engine/render/renderdevice.hpp"

#include "engine/render/3d/forwardpipeline.hpp"

#include "engine/math/rotation.hpp"

const char *SHADER_VERT_OUTLINE_DEFAULT = R"###(
float4x4 MANA_M;
float4x4 MANA_V;
float4x4 MANA_P;
float4x4 MANA_MVP;
float4x4 MANA_M_INVERT;
float3 COLOR_OUTLINE;

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

const char *SHADER_FRAG_OUTLINE_DEFAULT = R"###(
float4x4 MANA_M;
float4x4 MANA_V;
float4x4 MANA_P;
float4x4 MANA_MVP;
float4x4 MANA_M_INVERT;
float3 COLOR_OUTLINE;

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 FragColor: SV_TARGET;
};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.FragColor = float4(COLOR_OUTLINE.x, COLOR_OUTLINE.y, COLOR_OUTLINE.z, 1.0);
    return ret;
}
)###";

namespace mana {
    ShaderProgram *getDefaultOutlineShader(RenderDevice &device) {
        return device.createShaderProgram(SHADER_VERT_OUTLINE_DEFAULT,
                                          SHADER_FRAG_OUTLINE_DEFAULT,
                                          {},
                                          {});
    }

    ForwardPipeline::ForwardPipeline(Renderer* ren) : ren(ren) {}

    ForwardPipeline::~ForwardPipeline() = default;

    void ForwardPipeline::render(RenderTarget &screen, RenderScene &scene) {
        Mat4f model, view, projection, cameraTranslation;
        view = scene.camera.view();
        projection = scene.camera.projection();
        cameraTranslation = MatrixMath::translate(scene.camera.transform.position);

        ren->renderBegin(screen);

        for (auto &unit : scene.forward) {
            model = MatrixMath::translate(unit.transform.position);
            model = model * MatrixMath::scale(unit.transform.scale);
            model = model * MatrixMath::rotate(unit.transform.rotation);

            ShaderProgram &shader = *unit.command.shader;

            shader.setMat4("MANA_M", model);
            shader.setMat4("MANA_V", view);
            shader.setMat4("MANA_P", projection);
            shader.setMat4("MANA_MVP", projection * view * model);
            shader.setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
            shader.setMat4("MANA_VIEW_TRANSLATION", cameraTranslation);

            int dirCount = 0;
            int pointCount = 0;
            int spotCount = 0;

            int i = 0;
            for (auto &light : scene.lights) {
                std::string name;
                switch (light.type) {
                    case LIGHT_DIRECTIONAL:
                        name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "direction", light.direction);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        dirCount++;
                        break;
                    case LIGHT_POINT:
                        name = "MANA_LIGHTS_POINT[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "position", light.transform.position);
                        shader.setFloat(name + "constantValue", light.constant);
                        shader.setFloat(name + "linearValue", light.linear);
                        shader.setFloat(name + "quadraticValue", light.quadratic);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        pointCount++;
                        break;
                    case LIGHT_SPOT:
                        name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
                        shader.setVec3(name + "position", light.transform.position);
                        shader.setVec3(name + "direction", light.direction);
                        shader.setFloat(name + "cutOff", cosf(degreesToRadians(light.cutOff)));
                        shader.setFloat(name + "outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                        shader.setFloat(name + "constantValue", light.constant);
                        shader.setFloat(name + "linearValue", light.linear);
                        shader.setFloat(name + "quadraticValue", light.quadratic);
                        shader.setVec3(name + "ambient", light.ambient);
                        shader.setVec3(name + "diffuse", light.diffuse);
                        shader.setVec3(name + "specular", light.specular);
                        spotCount++;
                        break;
                }
            }

            shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", dirCount);
            shader.setInt("MANA_LIGHT_COUNT_POINT", pointCount);
            shader.setInt("MANA_LIGHT_COUNT_SPOT", spotCount);

            shader.setVec3("MANA_VIEWPOS", unit.transform.position);

            ren->addCommand(unit.command);
        }

        ren->renderFinish();
    }
}