//--------------------------------------------------------------------------------------
// Pixel shader that used by all light sources
//--------------------------------------------------------------------------------------

#include "Common.hlsli" 


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------

Texture2D    DiffuseMap : register(t0);
SamplerState TexSampler : register(s0); 


//--------------------------------------------------------------------------------------
// Shader code
//--------------------------------------------------------------------------------------
float4 main(SimplePixelShaderInput input) : SV_Target
{

    float3 diffuseMapColour = DiffuseMap.Sample(TexSampler, input.uv).rgb;

    float3 finalColour = gObjectColour * diffuseMapColour;

    return float4(finalColour, 1.0f);
}