#include "mana.hlsl"

TextureCube skybox;

struct PS_INPUT {
    float3 fPos: POSITION;
};

struct PS_OUTPUT {
    float4 FragColor: SV_TARGET;
};

SamplerState samplerState_skybox
{};

PS_OUTPUT main(PS_INPUT v) {
    PS_OUTPUT ret;
    ret.FragColor = skybox.Sample(samplerState_skybox, v.fPos);
    return ret;
}