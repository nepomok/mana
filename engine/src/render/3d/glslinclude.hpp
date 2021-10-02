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

#ifndef MANA_GLSLINCLUDE_HPP
#define MANA_GLSLINCLUDE_HPP


/**
 * The hlsl source packaged with the binary which is injected when SHADER_INCLUDE is found in a user shader,
 * the hlsl preprocessor could also look for the include and read it from a file relative to the binary.
 */
static const char *SHADER_MANA_GLSL = R"###(
mat4 MANA_M;
mat4 MANA_V;
mat4 MANA_P;
mat4 MANA_MVP;

mat4 MANA_M_INVERT;

vec3 MANA_VIEWPOS;
mat4 MANA_VIEW_TRANSLATION;

struct MANA_T_LIGHT_DIRECTIONAL {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

MANA_T_LIGHT_DIRECTIONAL MANA_LIGHTS_DIRECTIONAL[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_DIRECTIONAL;

struct MANA_T_LIGHT_POINT {
    vec3 position;
    float constantValue;
    float linearValue;
    float quadraticValue;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

MANA_T_LIGHT_POINT MANA_LIGHTS_POINT[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_POINT;

struct MANA_T_LIGHT_SPOT {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constantValue;
    float linearValue;
    float quadraticValue;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

MANA_T_LIGHT_SPOT MANA_LIGHTS_SPOT[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_SPOT;

struct LightComponents
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

LightComponents mana_calculate_light_directional(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    LightComponents ret;

    for (int i = 0; i < MANA_LIGHT_COUNT_DIRECTIONAL; i++)
    {
        vec3 ambient = MANA_LIGHTS_DIRECTIONAL[i].ambient * vec3(diffuseColor.xyz);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(-MANA_LIGHTS_DIRECTIONAL[i].direction);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_DIRECTIONAL[i].diffuse * vec3((diff * diffuseColor).xyz);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_DIRECTIONAL[i].specular * vec3((spec * specularColor).xyz);

        ret.ambient += ambient;
        ret.diffuse += diffuse;
        ret.specular += specular;
    }

    return ret;
}

LightComponents mana_calculate_light_point(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float shininess)
{
    LightComponents ret;

    for (int i = 0; i < MANA_LIGHT_COUNT_POINT; i++)
    {
        float distance    = length(MANA_LIGHTS_POINT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_POINT[i].constantValue + MANA_LIGHTS_POINT[i].linearValue * distance + MANA_LIGHTS_POINT[i].quadraticValue * (distance * distance));

        vec3 ambient = MANA_LIGHTS_POINT[i].ambient * vec3(diffuseColor.xyz);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(MANA_LIGHTS_POINT[i].position - fPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_POINT[i].diffuse * vec3((diff * diffuseColor).xyz);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = MANA_LIGHTS_POINT[i].specular * vec3((spec * specularColor).xyz);

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        ret.ambient += ambient;
        ret.diffuse += diffuse;
        ret.specular += specular;
    }

    return ret;
}

LightComponents mana_calculate_light_spot(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    LightComponents ret;

    for (int i = 0; i < MANA_LIGHT_COUNT_SPOT; i++)
    {
        vec3 lightDir = normalize(MANA_LIGHTS_SPOT[i].position - fPos);

        vec3 ambient = MANA_LIGHTS_SPOT[i].ambient * diffuseColor.rgb;

        vec3 norm = normalize(fNorm);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = MANA_LIGHTS_SPOT[i].diffuse * diff * diffuseColor.rgb;

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_SPOT[i].specular * spec * specularColor.rgb;

        float theta = dot(lightDir, normalize(-MANA_LIGHTS_SPOT[i].direction));
        float epsilon = (MANA_LIGHTS_SPOT[i].cutOff - MANA_LIGHTS_SPOT[i].outerCutOff);
        float intensity = clamp((theta - MANA_LIGHTS_SPOT[i].outerCutOff) / epsilon, 0.0, 1.0);

        diffuse  *= intensity;
        specular *= intensity;

        float distance    = length(MANA_LIGHTS_SPOT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_SPOT[i].constantValue + MANA_LIGHTS_SPOT[i].linearValue * distance + MANA_LIGHTS_SPOT[i].quadraticValue * (distance * distance));

        diffuse   *= attenuation;
        specular *= attenuation;

        ret.ambient += ambient;
        ret.diffuse += diffuse;
        ret.specular += specular;
    }

    return ret;
}

LightComponents mana_calculate_light(vec3 fPos, vec3 fNorm, vec4 fDiffuse, vec4 fSpecular, float roughness)
{
    LightComponents dirLight = mana_calculate_light_directional(fPos, fNorm, fDiffuse, fSpecular, roughness);
    LightComponents pointLight = mana_calculate_light_point(fPos, fNorm, fDiffuse, fSpecular, roughness);
    LightComponents spotLight = mana_calculate_light_spot(fPos, fNorm, fDiffuse, fSpecular, roughness);

    LightComponents ret;
    ret.ambient = dirLight.ambient + pointLight.ambient + spotLight.ambient;
    ret.diffuse = dirLight.diffuse + pointLight.diffuse + spotLight.diffuse;
    ret.specular = dirLight.specular + pointLight.specular + spotLight.specular;
    return ret;
}
)###";

#endif //MANA_GLSLINCLUDE_HPP
