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

#include "engine/3d/passes/debugpass.hpp"

#include "engine/render/shadercompiler.hpp"

#include "engine/3d/renderer3d.hpp"

#include "engine/math/rotation.hpp"

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

out Vertex {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTan;
    vec3 vBitan;
    vec2 vUv;
} vs_out;

struct Globals {
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    float scale;
    int hasNormalTexture;
    int lightType;
};

uniform Globals globals;

void main()
{
    vs_out.vPos = vec4(position, 1);
    vs_out.vNorm = normal;
    vs_out.vTan = tangent;
    vs_out.vBitan = bitangent;
    vs_out.vUv = uv;
    gl_Position = vs_out.vPos;
}
)###";

static const char *SHADER_GEOMETRY = R"###(#version 460 core

layout(triangles) in;
layout(line_strip, max_vertices = 218) out;

in Vertex {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTan;
    vec3 vBitan;
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
    int lightType;
};

uniform Globals globals;

uniform sampler2D normal;

void drawLine(vec4 start, vec4 end)
{
    pos = start;
    gl_Position = pos;
    EmitVertex();
    pos = end;
    gl_Position = pos;
    EmitVertex();
    EndPrimitive();
}

void drawTextureNormal(vec4 vPos,
                vec3 vNorm,
                vec3 vTan,
                vec3 vBitan,
                vec2 vUv)
{
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    // Draw texture normals at vertex positions, normals for fragments between vertices are not drawn
    vec3 T = normalize(vTan);
    vec3 B = normalize(vBitan);
    vec3 N = normalize(vNorm);

    mat3 TBN = mat3(T, B, N);

    vec3 tangentNormal = texture(normal, vUv).xyz;
    tangentNormal = normalize((tangentNormal * 2) - 1);

    //Transform tangent space normal into local space
    vec3 localNormal = TBN * (tangentNormal * 0.02f);

    //Transform local space normal into clip space
    vec4 clipNormal = mvp * vec4((vec4(localNormal, 1) + vPos).xyz, 1);
    vec4 clipPos =  mvp * vPos;

    color = vec4(0.3, 1, 1, 1);
    drawLine(clipPos, clipNormal);
}

void drawVertexNormal(vec4 vPos,
                        vec3 vNorm,
                        vec3 vTan,
                        vec3 vBitan,
                        vec2 vUv)
{
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    vec4 cPos = mvp * vPos;
    vec4 cNorm = mvp * vec4((vec4(vNorm * globals.scale, 0) + vPos).xyz, 1);
    vec4 cTang = mvp * vec4((vec4(vTan * globals.scale, 0) + vPos).xyz, 1);
    vec4 cBitang = mvp * vec4((vec4(vBitan * globals.scale, 0) + vPos).xyz, 1);

    // Draw vertex normal, tangent and bitangent
    //Normal
    color = vec4(0, 0, 1, 1);
    drawLine(cPos, cNorm);

    //Tangent
    color = vec4(1, 0, 0, 1);
    drawLine(cPos, cTang);

    //Bitangent
    color = vec4(0, 1, 0, 1);
    drawLine(cPos, cBitang);
}

void drawInterpolatedVertex(vec2 position)
{
    vec4 vPos = mix(mix(gs_in[0].vPos, gs_in[1].vPos, position.x), gs_in[2].vPos, position.y);
    vec3 vNorm = mix(mix(gs_in[0].vNorm, gs_in[1].vNorm, position.x), gs_in[2].vNorm, position.y);
    vec3 vTan = mix(mix(gs_in[0].vTan, gs_in[1].vTan, position.x), gs_in[2].vTan, position.y);
    vec3 vBitan = mix(mix(gs_in[0].vBitan, gs_in[1].vBitan, position.x), gs_in[2].vBitan, position.y);
    vec2 vUv = mix(mix(gs_in[0].vUv, gs_in[1].vUv, position.x), gs_in[2].vUv, position.y);
    drawTextureNormal(vPos, vNorm, vTan, vBitan, vUv);
}

void main()
{
    //Draw normal direction vectors for each vertex in the triangle
    for(uint i = 0; i < 3; i++)
    {
        drawVertexNormal(gs_in[i].vPos, gs_in[i].vNorm, gs_in[i].vTan, gs_in[i].vBitan, gs_in[i].vUv);
    }

    if (globals.hasNormalTexture == 1)
    {
        //Draw surface normals
        for (int x = 0; x < 5; x++)
        {
            for (int y = 0; y < 5; y++)
            {
                drawInterpolatedVertex(vec2(x / 5.0f, y / 5.0f));
            }
        }
    }
}
)###";

static const char *SHADER_VERT_LIGHT = R"###(#version 460 core

#include "mana.glsl"

struct Globals {
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    int lightType;
    MANA_T_LIGHT_DIRECTIONAL dirLight;
    MANA_T_LIGHT_POINT pointLight;
    MANA_T_LIGHT_SPOT spotLight;
};

uniform Globals globals;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout (location = 5) in vec4 instanceRow0;
layout (location = 6) in vec4 instanceRow1;
layout (location = 7) in vec4 instanceRow2;
layout (location = 8) in vec4 instanceRow3;

out Vertex {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTan;
    vec3 vBitan;
    vec2 vUv;
} vs_out;

void main()
{
    vs_out.vPos = vec4(position, 1);
    vs_out.vNorm = normal;
    vs_out.vTan = tangent;
    vs_out.vBitan = bitangent;
    vs_out.vUv = uv;
    gl_Position = vs_out.vPos;
}
)###";

static const char *SHADER_GEOMETRY_LIGHT = R"###(#version 460 core

#include "mana.glsl"

struct Globals {
    mat4 MODEL;
    mat4 VIEW;
    mat4 PROJECTION;
    int lightType;
    MANA_T_LIGHT_DIRECTIONAL dirLight;
    MANA_T_LIGHT_POINT pointLight;
    MANA_T_LIGHT_SPOT spotLight;
};

uniform Globals globals;

layout(triangles) in;
layout(line_strip, max_vertices = 16) out;

in Vertex {
    vec4 vPos;
    vec3 vNorm;
    vec3 vTan;
    vec3 vBitan;
    vec2 vUv;
} gs_in[];

layout(location = 0) out vec4 pos;
layout(location = 1) out vec4 color;

void drawLine(vec4 start, vec4 end)
{
    pos = start;
    gl_Position = pos;
    EmitVertex();
    pos = end;
    gl_Position = pos;
    EmitVertex();
    EndPrimitive();
}

void drawDirLight()
{
    color = vec4(1, 1, 0, 1);
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    float baseScale = 0.5f;
    vec3 dir = globals.dirLight.direction;
    vec3 tan = normalize(cross(globals.dirLight.direction, vec3(0.0,0.0,1.0))) * baseScale;
    vec3 bitan = normalize(cross(dir, tan)) * baseScale;

    drawLine(mvp * vec4(0, 0, 0, 1), mvp * vec4(dir.xyz, 1));

    drawLine(mvp * vec4(-(tan / 2).xyz, 1), mvp * vec4((tan / 2).xyz, 1));
    drawLine(mvp * vec4(-(bitan / 2).xyz , 1), mvp * vec4((bitan / 2).xyz, 1));
}

void drawPointLight()
{
    color = vec4(1, 1, 0, 1);
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    float scale = 1;

    vec3 norm = vec3(0, 1, 0);
    vec3 tan = cross(norm, vec3(0.0,0.0,1.0));
    vec3 bitan = cross(norm, tan);

    vec4 clipOrigin = mvp * vec4(0, 0, 0, 1);

    drawLine(clipOrigin, mvp * vec4(norm.xyz * scale, 1));
    drawLine(clipOrigin, mvp * vec4(-norm.xyz * scale, 1));

    drawLine(clipOrigin, mvp * vec4(tan.xyz * scale, 1));
    drawLine(clipOrigin, mvp * vec4(-tan.xyz * scale, 1));

    drawLine(clipOrigin, mvp * vec4(bitan.xyz * scale, 1));
    drawLine(clipOrigin, mvp * vec4(-bitan.xyz * scale, 1));
}

void drawSpotLight()
{
    color = vec4(1, 1, 0, 1);
    mat4 mvp = globals.PROJECTION * globals.VIEW * globals.MODEL;

    float scale = 1;

    vec3 norm = normalize(globals.spotLight.direction);
    vec3 c1 = normalize(cross(norm, vec3(0.0, 0.0, 1.0)));
    vec3 c2 = normalize(cross(norm, vec3(0.0, 1.0, 0.0)));

    vec3 tan;
    if(length(c1) > length(c2))
    {
        tan = c1;
    }
    else
    {
        tan = c2;
    }

    vec3 bitan = normalize(cross(norm, tan));

    float coneRadius = 0.2f;

    vec4 clipOrigin = mvp * vec4(0, 0, 0, 1);
    vec4 clipNorm = mvp * vec4(norm.xyz * scale, 1);
    vec4 clipTailTan = mvp * vec4((norm * scale + tan * coneRadius).xyz, 1);
    vec4 clipTailBitan = mvp * vec4((norm * scale + bitan * coneRadius).xyz, 1);
    vec4 clipTailTanEnd = mvp * vec4((norm * scale + -(tan * coneRadius)).xyz, 1);
    vec4 clipTailBitanEnd = mvp * vec4((norm * scale + -(bitan * coneRadius)).xyz, 1);

    drawLine(clipOrigin, clipNorm);
    drawLine(clipTailTan, clipTailTanEnd);
    drawLine(clipTailBitan, clipTailBitanEnd);
    drawLine(clipTailTan, clipOrigin);
    drawLine(clipTailTanEnd, clipOrigin);
    drawLine(clipTailBitan, clipOrigin);
    drawLine(clipTailBitanEnd, clipOrigin);
}

void main()
{
    switch(globals.lightType)
    {
        case 0:
            drawDirLight();
            break;
        case 1:
            drawPointLight();
            break;
        case 2:
            drawSpotLight();
            break;
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

        ShaderSource vsl(SHADER_VERT_LIGHT, "main", VERTEX, GLSL_460);
        ShaderSource gsl(SHADER_GEOMETRY_LIGHT, "main", GEOMETRY, GLSL_460);

        vsl.preprocess(Renderer3D::getShaderIncludeCallback(), Renderer3D::getShaderMacros(GLSL_460));
        gsl.preprocess(Renderer3D::getShaderIncludeCallback(), Renderer3D::getShaderMacros(GLSL_460));

        // Compile preprocessed glsl into spirv and back into glsl to get rid of unnecessary incompatible preprocessor statements added by shaderc when including in glsl.
        vsl = ShaderSource(ShaderCompiler::decompileSPIRV(vsl.compile(), ShaderLanguage::GLSL_460),
                           "main",
                           VERTEX,
                           GLSL_460);

        gsl = ShaderSource(ShaderCompiler::decompileSPIRV(gsl.compile(), ShaderLanguage::GLSL_460),
                           "main",
                           GEOMETRY,
                           GLSL_460);

        shader = device.getAllocator().createShaderProgram(vs, gs, fs);
        shaderLight = device.getAllocator().createShaderProgram(vsl, gsl, fs);

        meshBuffer = device.getAllocator().createMeshBuffer(Mesh(Mesh::TRI, {Vertex(Vec3f(0))}, {0, 0, 0}));
    }

    DebugPass::~DebugPass() {
        delete shader;
        delete shaderLight;
        delete meshBuffer;
    }

    void DebugPass::prepareBuffer(GeometryBuffer &gBuffer) {
        gBuffer.addBuffer("debug", TextureBuffer::RGBA);
    }

    void DebugPass::render(GeometryBuffer &gBuffer, Scene &scene) {
        gBuffer.attachColor({"debug"});
        gBuffer.attachDepthStencil("depth");

        auto &ren = device.getRenderer();

        ren.renderBegin(gBuffer.getRenderTarget(),
                        RenderOptions({},
                                      gBuffer.getSize(),
                                      false,
                                      {},
                                      1,
                                      true,
                                      false,
                                      false));

        if (drawNormals) {
            for (auto &deferredCommand: scene.deferred) {
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

        if (drawLights) {
            for (auto &light: scene.lights) {
                RenderCommand command;
                command.shader = shaderLight;
                command.meshBuffers.emplace_back(meshBuffer);

                command.shader->setMat4("globals.MODEL", light.transform.model());
                command.shader->setMat4("globals.VIEW", scene.camera.view());
                command.shader->setMat4("globals.PROJECTION", scene.camera.projection());

                std::string prefix;
                switch (light.type) {
                    case LIGHT_DIRECTIONAL:
                        prefix = "globals.dirLight";
                        command.shader->setVec3(prefix + ".direction", light.direction);
                        command.shader->setInt("globals.lightType", 0);
                        break;
                    case LIGHT_POINT:
                        prefix = "globals.pointLight";
                        command.shader->setInt("globals.lightType", 1);
                        break;
                    case LIGHT_SPOT:
                        prefix = "globals.spotLight";
                        command.shader->setVec3(prefix + ".direction", light.direction);
                        command.shader->setFloat(prefix + ".cutOff", cosf(degreesToRadians(light.cutOff)));
                        command.shader->setFloat(prefix + ".outerCutOff", cosf(degreesToRadians(light.outerCutOff)));
                        command.shader->setInt("globals.lightType", 2);
                        break;
                }

                ren.addCommand(command);
            }
        }

        ren.renderFinish();
    }

    void DebugPass::setDrawNormals(bool value) {
        drawNormals = value;
    }

    void DebugPass::setDrawLightCasters(bool value) {
        drawLights = value;
    }

}