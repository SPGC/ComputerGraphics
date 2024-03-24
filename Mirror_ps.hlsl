//--------------------------------------------------------------------------------------
// Pixel shader for mirror
//--------------------------------------------------------------------------------------

#include "Common.hlsli" 


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------

Texture2D DiffuseSpecularMap : register(t0); 
SamplerState TexSampler : register(s0); 


//--------------------------------------------------------------------------------------
// Shader code
//--------------------------------------------------------------------------------------

float4 main(LightingPixelShaderInput input) : SV_Target
{
    input.worldNormal = normalize(input.worldNormal); 
    float3 cameraDirection = normalize(gCameraPositionR4 - input.worldPosition);

    // Light 1
    float3 light1Vector = gLightPulsatingPositionR4 - input.worldPosition;
    float3 light1Dist = length(light1Vector);
    float3 light1Direction = light1Vector / light1Dist;
    float3 diffuseLight1 = gLightPulsatingColourR4 * max(dot(input.worldNormal, light1Direction), 0) / light1Dist; 
    
    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR4); 

    // Light 2
    float3 light2Vector = gLightGradientPositionR4 - input.worldPosition;
    float3 light2Dist = length(light2Vector);
    float3 light2Direction = light2Vector / light2Dist;
    float3 diffuseLight2 = gLightGradientColourR4 * max(dot(input.worldNormal, light2Direction), 0) / light2Dist;

    halfway = normalize(light2Direction + cameraDirection);
    float3 specularLight2 = diffuseLight2 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR4);

    // Here I invert texture in horizontal so it looks like mirror
    float4 textureColour = DiffuseSpecularMap.Sample(TexSampler, float2(1 - input.uv.x, input.uv.y));
    float3 diffuseMaterialColour = textureColour.rgb;
    float specularMaterialColour = textureColour.a; 

    // Combine lighting with texture colours
    float3 finalColour = ((gAmbientColourR4 + diffuseLight1 + diffuseLight2) * diffuseMaterialColour +
                         (specularLight1 + specularLight2) * specularMaterialColour);
    
    return float4(finalColour, 1.0f);
}
