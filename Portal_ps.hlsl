//--------------------------------------------------------------------------------------
// Pixel shader for TV (portal)
//--------------------------------------------------------------------------------------


#include "Common.hlsli" // Shaders can also use include files - note the extension


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------

Texture2D DiffuseSpecularMap : register(t0); 
Texture2D TVTexture : register(t1);
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

    // Here two things are done: 
    // 1. Converting portal texture to grey scale
    // 2. Rendering portal texture only in places where TV texture has alpha channel == 0
    float4 TVTextureColour = TVTexture.Sample(TexSampler, input.uv);
    float3 materialColour = TVTextureColour.rgb;
    float screenFlag = TVTextureColour.a;
    float4 textureColour = DiffuseSpecularMap.Sample(TexSampler, input.uv);
    float meanValue = (textureColour.r + textureColour.g + textureColour.b) / 3;
    float3 diffuseMaterialColour = { meanValue, meanValue, meanValue }; 
    float specularMaterialColour = textureColour.a; 

    // Combine lighting with texture colours
    float3 finalColour = ((gAmbientColourR4 + diffuseLight1 + diffuseLight2) * diffuseMaterialColour +
                         (specularLight1 + specularLight2) * specularMaterialColour) * (1 - screenFlag) +
                         ((gAmbientColourR4 + diffuseLight1 + diffuseLight2) * materialColour +
                         (specularLight1 + specularLight2)) * screenFlag;


    
    return float4(finalColour, 1.0f); 
}
