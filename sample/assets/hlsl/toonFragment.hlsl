//https://gamedev.net/forums/topic/566320-hlsl-cel-toon-shader-in-xna/4624466/
#include "mana.hlsl"

sampler2D tex0 : register(s0);
sampler1D diffuseRamp : register(s1);

struct PS_INPUT
{
   float3 pos : TEXCOORD0;
   float3 normal: 	TEXCOORD1;
   float2 uv: TEXCOORD2;
};

struct PS_OUTPUT
{
    float4 : COLOR0;
}

PS_OUTPUT main(PS_INPUT v)
{
    float3 eyeDir = normalize(MANA_VIEWPOS - v.pos.xyz);

    float3 lightPos = float3(0, 0, 0);
    float lightDir = normalize(lightPos - v.pos.xyz);

	float4 texCol = tex2D(tex0, v.uv);

	float diffuse = clamp(dot(v.normal, lightDir), 0.0, 1.0);

	float4 lightColor = tex1D(diffuseRamp, diffuse);

	float fresnal = dot(v.normal, eyeDir);
	fresnal = clamp((fresnal - 0.2) * 1000.0, 0.0, 1.0);
	lightColor *= fresnal;

	float3 reflect = (2.0 * diffuse * v.normal) - lightDir;

	float specular = pow(clamp(dot(reflect, eyeDir), 0.0, 1.0), 8.0);
	specular = clamp((specular - 0.5) * 1000.0, 0.0, 1.0);

	float4 specular4 = specular * float4(1.0, 1.0, 1.0, 1.0);

	return(saturate(lightColor * texCol + specular4));
}
