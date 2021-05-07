#include "mana.hlsl"

Texture2D diffuse;

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 FragColor: SV_TARGET;
};

SamplerState samplerState_diffuse
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    float4 tex = diffuse.Sample(samplerState_diffuse, v.fUv);
    ret.FragColor = float4(tex.r, tex.r, tex.r, 1);
    return ret;
}