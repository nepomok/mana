/**
 *  Mana - 3D Engine
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

#ifndef MANA_SHADERSOURCE_HPP
#define MANA_SHADERSOURCE_HPP

const char *vertexShader = R"###(
#version 330 core
#include "mana.glsl"

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 instanceMatrix;

out vec3 fPos;
out vec3 fNorm;
out vec2 fUv;

void main()
{
    fPos = vec3(MANA_M * instanceMatrix * vec4(position, 1.0));
    fNorm = vec3(transpose(inverse(MANA_M * instanceMatrix)) * vec4(normal, 1.0));
    fUv = uv;
    gl_Position = MANA_MVP * instanceMatrix * vec4(position, 1.0);
}
)###";

const char *fragmentShader = R"###(
#version 330 core
#include "mana.glsl"

uniform sampler2D diffuse;
uniform sampler2D specular;

in vec3 fPos;
in vec3 fNorm;
in vec2 fUv;

out vec4 FragColor;

void main()
{
    FragColor = MANA_F_CALCULATELIGHT(fPos, fNorm, texture(diffuse, fUv), texture(specular, fUv), 32);
}
)###";

const char *lightFragmentShader = R"###(
#version 330 core

in vec3 fpos;
in vec3 fnorm;
in vec2 fuv;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1,1,1,1);
}
)###";

const char *skyboxVertexShader = R"###(
#version 330 core
#include "mana.glsl"

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 instanceMatrix;

out vec3 TexCoords;

void main()
{
    TexCoords = position;
    gl_Position = MANA_MVP * vec4(position, 1.0);
}
)###";

const char *skyboxFragmentShader = R"###(
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, vec3(TexCoords.x, TexCoords.y, TexCoords.z));
}
)###";

#endif //MANA_SHADERSOURCE_HPP
