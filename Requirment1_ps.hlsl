//--------------------------------------------------------------------------------------
// Pixel Shader for Requirmenet1
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
    float3 cameraDirection = normalize(gCameraPositionR1 - input.worldPosition);

    // Light 1
    float distance1 = distance(gLightPulsatingPositionR1, input.worldPosition);
    float3 light1Direction = normalize(gLightPulsatingPositionR1 - input.worldPosition);
    float3 diffuseLight1 = gLightPulsatingColourR1 * max(dot(input.worldNormal, light1Direction), 0) / distance1;

    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR1) / distance1; 


    // Light 2
    float distance2 = distance(gLightGradientPositionR1, input.worldPosition);

    float3 light2Direction = normalize(gLightGradientPositionR1 - input.worldPosition);
    float3 diffuseLight2 = gLightGradientColourR1 * max(dot(input.worldNormal, light2Direction), 0) / distance2;

    halfway = normalize(light2Direction + cameraDirection);
    float3 specularLight2 = diffuseLight2 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR1) / distance2;


    // Combine lighting with texture colours
    float4 textureColour = DiffuseSpecularMap.Sample(TexSampler, input.uv);
    float3 diffuseMaterialColour = textureColour.rgb;
    float specularMaterialColour = textureColour.a;

    float3 finalColour = (gAmbientColourR1 + diffuseLight1 + diffuseLight2) * diffuseMaterialColour +
                         (specularLight1 + specularLight2) * specularMaterialColour;
    

    return float4(finalColour, 1.0f);
}