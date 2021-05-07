//https://gamedev.net/forums/topic/566320-hlsl-cel-toon-shader-in-xna/4624466/
#include "mana.hlsl"

struct VS_INPUT
{
    float3 position : POSITION0;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 instanceRow0 : POSITION1;
    float4 instanceRow1 : POSITION2;
    float4 instanceRow2 : POSITION3;
    float4 instanceRow3 : POSITION4;
};

struct VS_OUTPUT
{
   float4 Position: POSITION0;
   float3 fPos: TEXCOORD0;
   float3 Normal: 	TEXCOORD1;
   float2 TexCoord: TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT v)
{
   VS_OUTPUT ret;

   ret.Position = mul(v.Position, MANA_MVP);
   ret.fPos = mul(v.Position, MANA_M);
   ret.TexCoord = TexCoord;
   ret.Normal = normalize(v.Normal);

   return ret;
}