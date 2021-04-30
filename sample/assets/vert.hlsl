#include "mana.hlsl"

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
    float3  fPos : POSITION;
    float3  fNorm : NORMAL;
    float2  fUv : TEXCOORD0;
    float4 vPos : SV_Position;
};

VS_OUTPUT main(const VS_INPUT v)
{
    VS_OUTPUT ret;

    ret.vPos = mul(float4(v.position, 1), MANA_MVP);
    ret.fPos = mul(float4(v.position, 1), MANA_M).xyz;
    ret.fNorm = mul(float4(v.normal, 1), transpose(MANA_M_INVERT)).xyz;
    ret.fUv = v.uv;

    return ret;
}
