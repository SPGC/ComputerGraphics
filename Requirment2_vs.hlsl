//--------------------------------------------------------------------------------------
// Vertex Shader for Requirmenet2
//--------------------------------------------------------------------------------------

#include "Common.hlsli" // Shaders can also use include files - note the extension


//--------------------------------------------------------------------------------------
// Shader code
//-------------------------------------------------------------------------------------- 
LightingPixelShaderInput main(BasicVertex modelVertex)
{
    LightingPixelShaderInput output; 
    float4 modelPosition = float4(modelVertex.position, 1);

    float4 worldPosition = mul(gWorldMatrix, modelPosition);
    float4 modelNormal = float4(modelVertex.normal, 0);
    
    worldPosition.x += sin(modelPosition.y + gWiggleStateR2) * gWiggleScaleR2 * 5;
    worldPosition.y += sin(modelPosition.x + gWiggleStateR2) * gWiggleScaleR2 * 5;
    
    worldPosition += float4(mul(gWorldMatrix, modelNormal).xyz * (sin(gWiggleStateR2) + 10.0f) * gWiggleScaleR2, 0);
    
    float4 viewPosition = mul(gViewMatrix, worldPosition);
    output.projectedPosition = mul(gProjectionMatrix, viewPosition);

    output.worldNormal = mul(gWorldMatrix, modelNormal).xyz; 
    output.worldPosition = worldPosition.xyz; 
    output.uv = modelVertex.uv;

    return output; 
}
