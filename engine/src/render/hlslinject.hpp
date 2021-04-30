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

#ifndef MANA_HLSLINJECT_HPP
#define MANA_HLSLINJECT_HPP

#define SHADER_INCLUDE std::string("#include \"mana.hlsl\"\n")

/**
 * The hlsl source which is injected when SHADER_INCLUDE is found in a user shader.
 */
 const char *SHADER_INJECT = R"###(
float4x4 MANA_M;
float4x4 MANA_V;
float4x4 MANA_P;
float4x4 MANA_MVP;

float4x4 MANA_M_INVERT;

float3 MANA_VIEWPOS;

struct MANA_T_LIGHT_DIRECTIONAL {
    float3 direction;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

#define MANA_MAX_LIGHTS 20

MANA_T_LIGHT_DIRECTIONAL MANA_LIGHTS_DIRECTIONAL[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_DIRECTIONAL;

struct MANA_T_LIGHT_POINT {
    float3 position;
    float constantValue;
    float linearValue;
    float quadraticValue;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

MANA_T_LIGHT_POINT MANA_LIGHTS_POINT[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_POINT;

struct MANA_T_LIGHT_SPOT {
    float3 position;
    float3 direction;
    float cutOff;
    float outerCutOff;
    float constantValue;
    float linearValue;
    float quadraticValue;
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

MANA_T_LIGHT_SPOT MANA_LIGHTS_SPOT[MANA_MAX_LIGHTS];

int MANA_LIGHT_COUNT_SPOT;

float4 MANA_F_CALCULATELIGHT_DIRECTIONAL(float3 fPos, float3 fNorm, float4 diffuseColor, float4 specularColor, float roughness)
{
    float4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_DIRECTIONAL; i++)
    {
        float3 ambient = MANA_LIGHTS_DIRECTIONAL[i].ambient * float3(diffuseColor.xyz);

        float3 norm = normalize(fNorm);
        float3 lightDir = normalize(-MANA_LIGHTS_DIRECTIONAL[i].direction);

        float diff = max(dot(norm, lightDir), 0.0);
        float3 diffuse =  MANA_LIGHTS_DIRECTIONAL[i].diffuse * float3((diff * diffuseColor).xyz);

        float3 viewDir = normalize(MANA_VIEWPOS - fPos);
        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        float3 specular = MANA_LIGHTS_DIRECTIONAL[i].specular * float3((spec * specularColor).xyz);

        ret += float4(ambient + diffuse + specular, 1);
    }

    return ret;
}

float4 MANA_F_CALCULATELIGHT_POINT(float3 fPos, float3 fNorm, float4 diffuseColor, float4 specularColor, float roughness)
{
    float4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_POINT; i++)
    {
        float distance    = length(MANA_LIGHTS_POINT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_POINT[i].constantValue + MANA_LIGHTS_POINT[i].linearValue * distance + MANA_LIGHTS_POINT[i].quadraticValue * (distance * distance));

        float3 ambient = MANA_LIGHTS_POINT[i].ambient * float3(diffuseColor.xyz);

        float3 norm = normalize(fNorm);
        float3 lightDir = normalize(MANA_LIGHTS_POINT[i].position - fPos);

        float diff = max(dot(norm, lightDir), 0.0);
        float3 diffuse =  MANA_LIGHTS_POINT[i].diffuse * float3((diff * diffuseColor).xyz);

        float3 viewDir = normalize(MANA_VIEWPOS - fPos);
        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        float3 specular = MANA_LIGHTS_POINT[i].specular * float3((spec * specularColor).xyz);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        ret += float4(ambient + diffuse + specular, 1);
    }

    return ret;
}

float4 MANA_F_CALCULATELIGHT_SPOT(float3 fPos, float3 fNorm, float4 diffuseColor, float4 specularColor, float roughness)
{
    float4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_SPOT; i++)
    {
        float3 lightDir = normalize(MANA_LIGHTS_SPOT[i].position - fPos);

        // ambient
        float3 ambient = MANA_LIGHTS_SPOT[i].ambient * diffuseColor.rgb;

        // diffuse
        float3 norm = normalize(fNorm);
        float diff = max(dot(norm, lightDir), 0.0);
        float3 diffuse = MANA_LIGHTS_SPOT[i].diffuse * diff * diffuseColor.rgb;

        // specular
        float3 viewDir = normalize(MANA_VIEWPOS - fPos);
        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        float3 specular = MANA_LIGHTS_SPOT[i].specular * spec * specularColor.rgb;

        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-MANA_LIGHTS_SPOT[i].direction));
        float epsilon = (MANA_LIGHTS_SPOT[i].cutOff - MANA_LIGHTS_SPOT[i].outerCutOff);
        float intensity = clamp((theta - MANA_LIGHTS_SPOT[i].outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;

        // attenuation
        float distance    = length(MANA_LIGHTS_SPOT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_SPOT[i].constantValue + MANA_LIGHTS_SPOT[i].linearValue * distance + MANA_LIGHTS_SPOT[i].quadraticValue * (distance * distance));

        diffuse   *= attenuation;
        specular *= attenuation;

        float3 result = ambient + diffuse + specular;
        ret += float4(result, 1);
    }
    return ret;
}

float4 MANA_F_CALCULATELIGHT(float3 fPos, float3 fNorm, float4 fDiffuse, float4 fSpecular, float roughness)
{
    return MANA_F_CALCULATELIGHT_DIRECTIONAL(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_POINT(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_SPOT(fPos, fNorm, fDiffuse, fSpecular, roughness);
}
)###";

#endif //MANA_HLSLINJECT_HPP
