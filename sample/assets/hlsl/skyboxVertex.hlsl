#include "mana.hlsl"

struct VS_INPUT {
    float3 position: POSITION;
    float3 normal: NORMAL;
    float2 uv: TEXCOORD0;
};

struct VS_OUTPUT {
    float4 vPos: SV_POSITION;
    float3 texCoords: POSITION;
};

VS_OUTPUT main(const VS_INPUT v) {
    VS_OUTPUT ret;

    ret.vPos = mul(float4(v.position, 1), mul(MANA_VIEW_TRANSLATION, mul(MANA_V, MANA_P)));
    ret.texCoords = v.position;

    return ret;
}