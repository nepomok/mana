#include "mana.hlsl"

Texture2D diffuse;
Texture2D specular;

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 FragColor: SV_TARGET;
};

SamplerState TextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.FragColor = MANA_F_CALCULATELIGHT(v.fPos, v.fNorm, diffuse.Sample(TextureSampler, v.fUv), specular.Sample(TextureSampler, v.fUv), 32);
    return ret;
}
