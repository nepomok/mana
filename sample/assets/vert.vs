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
