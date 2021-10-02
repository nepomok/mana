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

#include "engine/render/3d/passes/debugpass.hpp"

#include "engine/render/shadercompiler.hpp"

static const char *SHADER_VERT = R"###(#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 instanceRow0;
layout (location = 6) in vec4 instanceRow1;
layout (location = 7) in vec4 instanceRow2;
layout (location = 8) in vec4 instanceRow3;

out VS_OUT {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTang;
    vec3 vBitang;
    vec2 vUv;
} vs_out;

struct Globals {
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    float scale;
    int hasNormalTexture;
};

uniform Globals globals;

void main()
{
    vs_out.vPos = vec4(position, 1);
    vs_out.vNorm = normal;
    vs_out.vTang = tangent;
    vs_out.vBitang = bitangent;
    vs_out.vUv = uv;
    gl_Position = vs_out.vPos;
}
)###";

static const char *SHADER_GEOMETRY = R"###(#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 18) out;

in VS_OUT {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTang;
    vec3 vBitang;
    vec2 vUv;
} gs_in[];

layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 color;

struct Globals {
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    float scale;
    int hasNormalTexture;
};

uniform Globals globals;

uniform sampler2D normal;

void main()
{
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    //Draw normal direction vectors for each vertex in the triangle
    for(uint i = 0; i < 3; i++)
    {
        vec4 vPos = gs_in[i].vPos;
        vec3 vNorm = gs_in[i].vNorm;
        vec3 vTang = gs_in[i].vTang;
        vec3 vBitang = gs_in[i].vBitang;
        vec2 vUv = gs_in[i].vUv;

        if (globals.hasNormalTexture == 1)
        {
            // Draw texture normals at vertex positions, normals for fragments between vertices are not drawn
            vec3 T = normalize(vTang);
            vec3 B = normalize(vBitang);
            vec3 N = normalize(vNorm);

            mat3 TBN = mat3(T, B, N);

            vec3 tangentNormal = texture(normal, vUv).xyz;
            tangentNormal = normalize((tangentNormal * 2) - 1);

            //Transform tangent space normal into local space
            vec3 localNormal = TBN * (tangentNormal * globals.scale);

            //Transform local space normal into clip space
            vec4 clipNormal = mvp * vec4((vec4(localNormal, 1) + vPos).xyz, 1);
            vec4 clipPos =  mvp * vPos;

            color = vec4(1, 1, 0, 1);
            pos = clipPos;
            gl_Position = pos;
            EmitVertex();

            pos = clipNormal;
            gl_Position = pos;
            EmitVertex();

            EndPrimitive();
        }
        else
        {
            vec4 cPos = mvp * vPos;
            vec4 cNorm = mvp * vec4((vec4(vNorm * globals.scale, 0) + vPos).xyz, 1);
            vec4 cTang = mvp * vec4((vec4(vTang * globals.scale, 0) + vPos).xyz, 1);
            vec4 cBitang = mvp * vec4((vec4(vBitang * globals.scale, 0) + vPos).xyz, 1);

            // Draw vertex normal, tangent and bitangent
            //Normal
            color = vec4(0, 0, 1, 1);
            pos = cPos;
            gl_Position = pos;
            EmitVertex();
            pos = cNorm;
            gl_Position = pos;
            EmitVertex();
            EndPrimitive();

            //Tangent
            color = vec4(1, 0, 0, 1);
            pos = cPos;
            EmitVertex();
            pos = cTang;
            EmitVertex();
            EndPrimitive();

            //Bitangent
            color = vec4(0, 1, 0, 1);
            pos = cPos;
            gl_Position = pos;
            EmitVertex();
            pos = cBitang;
            gl_Position = pos;
            EmitVertex();
            EndPrimitive();
        }
    }
}
)###";

static const char *SHADER_FRAG = R"###(#version 460 core

layout(location = 0) in vec4 pos;
layout(location = 1) in vec4 color;

layout(location = 0) out vec4 fragColor;

void main()
{
    fragColor = color;
}
)###";

namespace engine {
    using namespace ShaderCompiler;

    DebugPass::DebugPass(RenderDevice &device)
            : device(device),
              drawNormals(false) {
        ShaderSource vs(SHADER_VERT, "main", VERTEX, GLSL_460);
        ShaderSource gs(SHADER_GEOMETRY, "main", GEOMETRY, GLSL_460);
        ShaderSource fs(SHADER_FRAG, "main", FRAGMENT, GLSL_460);
        shader = device.getAllocator().createShaderProgram(vs, gs, fs);
    }

    DebugPass::~DebugPass() {
        delete shader;
    }

    void DebugPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer("debug", TextureBuffer::RGBA);
    }

    void DebugPass::render(GeometryBuffer &gBuffer, RenderScene &scene) {
        gBuffer.attachColor({"debug"});
        gBuffer.attachDepthStencil("depth");

        auto &ren = device.getRenderer();

        ren.renderBegin(gBuffer.getRenderTarget(),
                        RenderOptions({},
                                      gBuffer.getSize(),
                                      false,
                                      {},
                                      true,
                                      false,
                                      false));

        if (drawNormals) {
            for (auto &deferredCommand : scene.deferred) {
                if (!shader->setFloat("globals.scale", 0.1f))
                    throw std::runtime_error("");
                shader->setMat4("globals.MODEL", deferredCommand.transform.model());
                shader->setMat4("globals.VIEW", scene.camera.view());
                shader->setMat4("globals.PROJECTION", scene.camera.projection());

                RenderCommand command;
                command.shader = shader;
                command.meshBuffers.emplace_back(deferredCommand.meshBuffer);
                if (deferredCommand.material.normalTexture != nullptr) {}
                command.properties.depthTestWrite = false;

                if (deferredCommand.material.normalTexture != nullptr) {
                    shader->setBool("globals.hasNormalTexture", true);
                    shader->setTexture("normal", 0);
                    command.textures.emplace_back(deferredCommand.material.normalTexture);
                } else {
                    shader->setBool("globals.hasNormalTexture", false);
                }

                ren.addCommand(command);
            }
        }

        ren.renderFinish();
    }

    void DebugPass::setDrawNormals(bool value) {
        drawNormals = value;
    }
}