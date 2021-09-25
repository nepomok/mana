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

#include "debugrenderer.hpp"

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
};

float4x4 mvp;
float4 color;

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;
    ret.vPos = mul(float4(v.position, 1), mvp);
    return ret;
}
)###";

static const char *SHADER_FRAG = R"###(
struct PS_OUTPUT {
    float4 color     :   SV_TARGET0;
};

float4x4 mvp;
float4 color;

PS_OUTPUT main() {
    PS_OUTPUT ret;
    ret.color = color;
    return ret;
}
)###";

namespace engine {
    DebugRenderer::DebugRenderer(RenderDevice *device) : device(device) {
        shader = device->getAllocator().createShaderProgram(SHADER_VERT, SHADER_FRAG, {}, {});
    }

    DebugRenderer::~DebugRenderer() {
        delete shader;
    }

    void DebugRenderer::renderBegin(RenderTarget &target) const {
        auto &ren = device->getRenderer();
        ren.renderBegin(target, RenderOptions({}, target.getSize(), false, {}, false, false, false));
    }

    void DebugRenderer::drawLine(Vec3f start, Vec3f end, ColorRGBA color, const Mat4f &mvp) {
        auto &ren = device->getRenderer();

        Mesh mesh(Mesh::LINE, {
                Vertex(start, {}),
                Vertex(end, {})
        });

        auto *meshBuffer = device->getAllocator().createMeshBuffer(mesh);

        meshBuffers.insert(meshBuffer);

        RenderCommand command;
        command.shader = shader;

        shader->setMat4("mvp", mvp);
        shader->setVec4("color", Vec4f((float) color.r() / 255,
                                       (float) color.g() / 255,
                                       (float) color.b() / 255,
                                       (float) color.a() / 255));

        command.meshBuffers.emplace_back(meshBuffer);

        ren.addCommand(command);
    }

    void DebugRenderer::drawNormalVectors(const Camera &camera,
                                          const std::vector<std::pair<Transform, Mesh>> &meshes,
                                          float scale) {
        for (auto &pair : meshes) {
            auto model = pair.first.model();
            auto view = camera.view();
            auto projection = camera.projection();
            auto mvp = projection * view * model;
            for (auto &vertex : pair.second.vertices) {
                auto position = vertex.position();
                auto normal = vertex.normal();
                auto tangent = vertex.tangent();
                auto bitangent = vertex.bitangent();
                drawLine(position, position + normal * scale, {0, 0, 255, 255}, mvp);
                drawLine(position, position + tangent * scale, {255, 0, 0, 255}, mvp);
                drawLine(position, position + bitangent * scale, {0, 255, 0, 255}, mvp);
            }
        }
    }


    void DebugRenderer::renderFinish() {
        auto &ren = device->getRenderer();
        ren.renderFinish();
        for (auto *ptr : meshBuffers)
            delete ptr;
        meshBuffers.clear();
    }
}