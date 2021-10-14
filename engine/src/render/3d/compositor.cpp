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

#include "engine/render/3d/compositor.hpp"


static const char *SHADER_VERT = R"###(#version 460 core

#define MAX_COLOR 15

struct Layer {
    sampler2D color[MAX_COLOR];
    int num_color;
    sampler2D depth;
    int has_depth;
};

struct Globals {
    Layer layer;
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

layout(location = 0) out vec4 fPos;
layout(location = 1) out vec2 fUv;

void main()
{
    fPos = vec4(position, 1);
    fUv = uv;
    gl_Position = fPos;
}

)###";

static const char *SHADER_FRAG = R"###(#version 460 core

#define MAX_COLOR 15

struct Layer {
    sampler2D color[MAX_COLOR];
    int num_color;
    sampler2D depth;
    int has_depth;
};

struct Globals {
    Layer layer;
};

uniform Globals globals;

layout(location = 0) in vec4 fPos;
layout(location = 1) in vec2 fUv;

layout(location = 0) out vec4 fragColor;

vec4 blend(vec4 colorA, vec4 colorB)
{
    return vec4((colorB.rgb * colorB.a + colorA.rgb * (1.0 - colorB.a)).rgb, 1);
}

void main()
{
    vec4 color = vec4(0, 0, 0, 0);
    float depth = 1;

    for (int ci = 0; ci < globals.layer.num_color; ci++)
    {
        color += texture(globals.layer.color[ci], fUv);
    }
    color.a = normalize(color.a);
    if (globals.layer.has_depth != 0)
    {
        depth = texture(globals.layer.depth, fUv).r;
    }
    else
    {
        depth = 1;
    }

    fragColor = color;
    gl_FragDepth = depth;
}
)###";

namespace engine {
    Compositor::Compositor(RenderDevice *device)
            : device(device) {
        ShaderSource shaderVert(SHADER_VERT, "main", ShaderCompiler::VERTEX, ShaderCompiler::GLSL_460);
        ShaderSource shaderFrag(SHADER_FRAG, "main", ShaderCompiler::FRAGMENT, ShaderCompiler::GLSL_460);
        shader = std::unique_ptr<ShaderProgram>(device->getAllocator().createShaderProgram(shaderVert, shaderFrag));
    }

    Compositor::Compositor(RenderDevice *device, std::vector<Layer> layers)
            : device(device), layers(std::move(layers)) {
        ShaderSource shaderVert(SHADER_VERT, "main", ShaderCompiler::VERTEX, ShaderCompiler::GLSL_460);
        ShaderSource shaderFrag(SHADER_FRAG, "main", ShaderCompiler::FRAGMENT, ShaderCompiler::GLSL_460);
        shader = std::unique_ptr<ShaderProgram>(device->getAllocator().createShaderProgram(shaderVert, shaderFrag));
    }

    std::vector<Compositor::Layer> &Compositor::getLayers() {
        return layers;
    }

    void Compositor::setClearColor(ColorRGB color) {
        clearColor = color;
    }

    void Compositor::presentLayers(RenderTarget &screen, GeometryBuffer &buffer) {
        presentLayers(screen, buffer, layers);
    }

    void Compositor::presentLayers(RenderTarget &screen,
                                   GeometryBuffer &buffer,
                                   const std::vector<Layer> &pLayers) {
        auto &ren = device->getRenderer();

        //Clear screen
        ren.renderBegin(screen, RenderOptions({}, screen.getSize(), false,
                                              {clearColor.r(), clearColor.g(), clearColor.b(), 255}));
        ren.renderFinish();

        if (layers.empty())
            return;

        for (auto &layer: layers) {
            drawLayer(screen, buffer, layer);
        }
    }

    void Compositor::drawLayer(RenderTarget &screen,
                               GeometryBuffer &buffer,
                               const Compositor::Layer &layer) {
        if (layer.color.size() > 14)
            throw std::runtime_error("Maximum of 15 color textures per layer");

        auto &ren = device->getRenderer();

        std::vector<TextureBuffer *> textures;
        std::string prefix = "globals.layer";
        shader->setInt(prefix + ".num_color", static_cast<int>(layer.color.size()));
        for (int ci = 0; ci < layer.color.size(); ci++) {
            shader->setTexture(prefix + ".color[" + std::to_string(ci) + "]", static_cast<int>(textures.size()));
            textures.emplace_back(&buffer.getBuffer(layer.color.at(ci)));
        }
        shader->setInt(prefix + ".has_depth", !layer.depth.empty());
        if (!layer.depth.empty()) {
            shader->setTexture(prefix + ".depth", static_cast<int>(textures.size()));
            textures.emplace_back(&buffer.getBuffer(layer.depth));
        }

        RenderCommand command;
        command.shader = shader.get();
        command.meshBuffers.emplace_back(&buffer.getScreenQuad());
        command.textures = textures;

        command.properties.enableBlending = true;
        command.properties.depthTestMode = layer.depthTestMode;
        command.properties.blendSourceMode = layer.colorBlendModeSource;
        command.properties.blendDestinationMode = layer.colorBlendModeDest;

        ren.renderBegin(screen, RenderOptions({}, screen.getSize(), false, {}, 0, false, false));
        ren.addCommand(command);
        ren.renderFinish();
    }
}