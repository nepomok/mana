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

SamplerState samplerState_diffuse
{};

SamplerState samplerState_specular
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.FragColor = mana_calculate_light(v.fPos,
                                          v.fNorm,
                                          diffuse.Sample(samplerState_diffuse,v.fUv),
                                          specular.Sample(samplerState_specular, v.fUv),
                                          32);
    return ret;
}
