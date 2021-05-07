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

#include "engine/render/3d/renderer3d.hpp"
#include "engine/math/rotation.hpp"

#include "hlslinject.hpp"

#include <algorithm>

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
    std::string includeCallback(const char *n) {
        std::string name(n);
        if (name == "mana.hlsl") {
            return SHADER_MANA;
        } else {
            throw std::runtime_error("Invalid name: " + name);
        }
    }

    const std::map<std::string, std::string> gMacros = {{"MANA_MAX_LIGHTS", "20"}};

    const std::function<std::string(const char *)> gIncludeFunc = {includeCallback};

    const std::map<std::string, std::string> &Renderer3D::getShaderMacros() {
        return gMacros;
    }

    const std::function<std::string(const char *)> &Renderer3D::getShaderIncludeCallback() {
        return gIncludeFunc;
    }

    Renderer3D::Renderer3D() : ren(nullptr), alloc(nullptr) {};

    Renderer3D::Renderer3D(Renderer &r, RenderAllocator &a) : ren(&r), alloc(&a) {}

    void Renderer3D::setEnableShadowMapping(bool shadowMapping) {
    }

    void Renderer3D::render(const RenderTarget &target,
                            const RenderScene &scene) {
        if (ren == nullptr || alloc == nullptr)
            throw std::runtime_error("Renderer 3d not initialized");

        bool outline = false;
        for (const auto &unit : scene.units) {
            if (unit.outline)
                outline = true;
        }

        Mat4f model, view, projection, camPosTransformMat;
        view = scene.camera->view();
        projection = scene.camera->projection();
        camPosTransformMat = MatrixMath::translate(scene.camera->transform.position);
        if (outline) {
            ShaderProgram *defaultOutlineShader = alloc->allocateShaderProgram(SHADER_VERT_OUTLINE_DEFAULT,
                                                                               SHADER_FRAG_OUTLINE_DEFAULT, {}, {});
            RenderScene sceneCopy = scene;
            ren->renderBegin(target);
            for (auto &unit : sceneCopy.units) {
                model = MatrixMath::translate(unit.transform.position);
                model = model * MatrixMath::scale(unit.transform.scale);
                model = model * MatrixMath::rotate(unit.transform.rotation);

                ShaderProgram &shader = *unit.command.shader;

                shader.setMat4("MANA_M", model);
                shader.setMat4("MANA_V", view);
                shader.setMat4("MANA_P", projection);
                shader.setMat4("MANA_MVP", projection * view * model);
                shader.setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
                shader.setMat4("MANA_VIEW_POSITION_MAT", camPosTransformMat);

                int i = 0;
                for (auto &light : scene.dir) {
                    std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "direction", light.direction);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }

                i = 0;
                for (auto &light : scene.point) {
                    std::string name = "MANA_LIGHTS_POINT[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "position", light.transform.position);
                    shader.setFloat(name + "constantValue", light.constant);
                    shader.setFloat(name + "linearValue", light.linear);
                    shader.setFloat(name + "quadraticValue", light.quadratic);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }
                i = 0;
                for (auto &light : scene.spot) {
                    std::string name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
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
                }

                shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", scene.dir.size());
                shader.setInt("MANA_LIGHT_COUNT_POINT", scene.point.size());
                shader.setInt("MANA_LIGHT_COUNT_SPOT", scene.spot.size());

                shader.setVec3("MANA_VIEWPOS", unit.transform.position);

                if (unit.outline) {
                    unit.command.properties.enableDepthTest = true;
                    unit.command.properties.enableStencilTest = true;
                    unit.command.properties.stencilFail = STENCIL_KEEP;
                    unit.command.properties.stencilDepthFail = STENCIL_KEEP;
                    unit.command.properties.stencilPass = STENCIL_REPLACE;
                    unit.command.properties.stencilTestMask = 0xFF;
                    unit.command.properties.stencilMode = STENCIL_ALWAYS;
                    unit.command.properties.stencilReference = 1;
                    unit.command.properties.stencilFunctionMask = 0xFF;
                } else {
                    unit.command.properties.enableStencilTest = false;
                    unit.command.properties.stencilTestMask = 0x00;
                    unit.command.properties.stencilFail = STENCIL_KEEP;
                    unit.command.properties.stencilDepthFail = STENCIL_KEEP;
                    unit.command.properties.stencilPass = STENCIL_KEEP;
                    unit.command.properties.stencilMode = STENCIL_NEVER;
                }

                ren->addCommand(unit.command);
            }
            for (auto &unit : sceneCopy.units) {
                if (unit.outline) {
                    unit.transform.scale *= unit.outlineScale;
                    if (unit.outlineShader == nullptr) {
                        unit.command.shader = defaultOutlineShader;
                    } else {
                        unit.command.shader = unit.outlineShader;
                    }

                    unit.command.properties.enableDepthTest = false;

                    unit.command.properties.stencilMode = STENCIL_NOTEQUAL;
                    unit.command.properties.stencilReference = 1;
                    unit.command.properties.stencilFunctionMask = 0xFF;

                    unit.command.properties.stencilTestMask = 0x00;

                    model = MatrixMath::translate(unit.transform.position);
                    model = model * MatrixMath::scale(unit.transform.scale);
                    model = model * MatrixMath::rotate(unit.transform.rotation);

                    ShaderProgram &shader = *unit.command.shader;

                    shader.setMat4("MANA_M", model);
                    shader.setMat4("MANA_V", view);
                    shader.setMat4("MANA_P", projection);
                    shader.setMat4("MANA_MVP", projection * view * model);
                    shader.setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
                    shader.setVec3("COLOR_OUTLINE", Vec3f((float) unit.outlineColor.r() / 255,
                                                          (float) unit.outlineColor.g() / 255,
                                                          (float) unit.outlineColor.b() / 255));

                    ren->addCommand(unit.command);
                }
            }
            ren->renderFinish();
            delete defaultOutlineShader;
        } else {
            ren->renderBegin(target);
            for (auto &unit : scene.units) {
                model = MatrixMath::translate(unit.transform.position);
                model = model * MatrixMath::scale(unit.transform.scale);
                model = model * MatrixMath::rotate(unit.transform.rotation);

                ShaderProgram &shader = *unit.command.shader;

                shader.setMat4("MANA_M", model);
                shader.setMat4("MANA_V", view);
                shader.setMat4("MANA_P", projection);
                shader.setMat4("MANA_MVP", projection * view * model);
                shader.setMat4("MANA_M_INVERT", MatrixMath::inverse(model));
                shader.setMat4("MANA_VIEW_POSITION_MAT", camPosTransformMat);

                int i = 0;
                for (auto &light : scene.dir) {
                    std::string name = "MANA_LIGHTS_DIRECTIONAL[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "direction", light.direction);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }

                i = 0;
                for (auto &light : scene.point) {
                    std::string name = "MANA_LIGHTS_POINT[" + std::to_string(i++) + "].";
                    shader.setVec3(name + "position", light.transform.position);
                    shader.setFloat(name + "constantValue", light.constant);
                    shader.setFloat(name + "linearValue", light.linear);
                    shader.setFloat(name + "quadraticValue", light.quadratic);
                    shader.setVec3(name + "ambient", light.ambient);
                    shader.setVec3(name + "diffuse", light.diffuse);
                    shader.setVec3(name + "specular", light.specular);
                }
                i = 0;
                for (auto &light : scene.spot) {
                    std::string name = "MANA_LIGHTS_SPOT[" + std::to_string(i++) + "].";
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
                }

                shader.setInt("MANA_LIGHT_COUNT_DIRECTIONAL", scene.dir.size());
                shader.setInt("MANA_LIGHT_COUNT_POINT", scene.point.size());
                shader.setInt("MANA_LIGHT_COUNT_SPOT", scene.spot.size());

                shader.setVec3("MANA_VIEWPOS", unit.transform.position);

                ren->addCommand(unit.command);
            }
            ren->renderFinish();
        }
    }

    const Renderer &Renderer3D::getRenderer() {
        assert(ren != nullptr);
        return *ren;
    }

    const RenderAllocator &Renderer3D::getAllocator() {
        assert(alloc != nullptr);
        return *alloc;
    }
}