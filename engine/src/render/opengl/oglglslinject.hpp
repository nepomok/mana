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

#ifndef MANA_OGLGLSLINJECT_HPP
#define MANA_OGLGLSLINJECT_HPP

#define SHADER_INCLUDE std::string("#include \"mana.glsl\"\n")

/**
 * The glsl source which is injected when SHADER_INCLUDE is found in a user shader.
 */
const char *SHADER_INJECT = R"###(
#define MANA_MAX_LIGHTS 10

uniform mat4 MANA_M;
uniform mat4 MANA_V;
uniform mat4 MANA_P;
uniform mat4 MANA_MVP;

uniform vec3 MANA_VIEWPOS;

struct MANA_T_LIGHT_DIRECTIONAL {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_DIRECTIONAL MANA_LIGHTS_DIRECTIONAL[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_DIRECTIONAL;

struct MANA_T_LIGHT_POINT {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_POINT MANA_LIGHTS_POINT[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_POINT;

struct MANA_T_LIGHT_SPOT {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_SPOT MANA_LIGHTS_SPOT[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_SPOT;

vec4 MANA_F_CALCULATELIGHT_DIRECTIONAL(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_DIRECTIONAL; i++)
    {
        vec3 ambient = MANA_LIGHTS_DIRECTIONAL[i].ambient * vec3(diffuseColor);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(-MANA_LIGHTS_DIRECTIONAL[i].direction);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_DIRECTIONAL[i].diffuse * vec3(diff * diffuseColor);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_DIRECTIONAL[i].specular * vec3(spec * specularColor);

        ret += vec4(ambient + diffuse + specular, 1);
    }

    return ret;
}

vec4 MANA_F_CALCULATELIGHT_POINT(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_POINT; i++)
    {
        float distance    = length(MANA_LIGHTS_POINT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_POINT[i].constant + MANA_LIGHTS_POINT[i].linear * distance + MANA_LIGHTS_POINT[i].quadratic * (distance * distance));

        vec3 ambient = MANA_LIGHTS_POINT[i].ambient * vec3(diffuseColor);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(MANA_LIGHTS_POINT[i].position - fPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_POINT[i].diffuse * vec3(diff * diffuseColor);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_POINT[i].specular * vec3(spec * specularColor);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        ret += vec4(ambient + diffuse + specular, 1);
    }

    return ret;
}

vec4 MANA_F_CALCULATELIGHT_SPOT(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_SPOT; i++)
    {
        vec3 lightDir = normalize(MANA_LIGHTS_SPOT[i].position - fPos);

        // ambient
        vec3 ambient = MANA_LIGHTS_SPOT[i].ambient * diffuseColor.rgb;

        // diffuse
        vec3 norm = normalize(fNorm);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = MANA_LIGHTS_SPOT[i].diffuse * diff * diffuseColor.rgb;

        // specular
        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_SPOT[i].specular * spec * specularColor.rgb;

        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-MANA_LIGHTS_SPOT[i].direction));
        float epsilon = (MANA_LIGHTS_SPOT[i].cutOff - MANA_LIGHTS_SPOT[i].outerCutOff);
        float intensity = clamp((theta - MANA_LIGHTS_SPOT[i].outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;

        // attenuation
        float distance    = length(MANA_LIGHTS_SPOT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_SPOT[i].constant + MANA_LIGHTS_SPOT[i].linear * distance + MANA_LIGHTS_SPOT[i].quadratic * (distance * distance));

        diffuse   *= attenuation;
        specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        ret += vec4(result, 1);
    }
    return ret;
}

vec4 MANA_F_CALCULATELIGHT(vec3 fPos, vec3 fNorm, vec4 fDiffuse, vec4 fSpecular, float roughness)
{
    return MANA_F_CALCULATELIGHT_DIRECTIONAL(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_POINT(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_SPOT(fPos, fNorm, fDiffuse, fSpecular, roughness);
}

)###";

#endif //MANA_OGLGLSLINJECT_HPP
