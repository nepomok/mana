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
