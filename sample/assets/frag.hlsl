#include "mana.hlsl"

Texture2D diffuse;
Texture2D specular;

struct PS_INPUT {
    float3 fPos: POSITION;
    float3 fNorm: NORMAL;
    float2 fUv: TEXCOORD0;
};

struct PS_OUTPUT {
    float4 FragColor: COLOR;
};

SamplerState TextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

PS_OUTPUT main(PS_INPUT input) {
    PS_OUTPUT ret;
    ret.FragColor = float4(0, 1, 0, 1);
    return ret;
}
