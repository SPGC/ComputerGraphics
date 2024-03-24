//--------------------------------------------------------------------------------------
// Pixel shader for cartoon Shrek
//--------------------------------------------------------------------------------------

#include "Common.hlsli" 


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------

Texture2D DiffuseMap : register(t0);
Texture2D CellMap : register(t1); 

SamplerState TexSampler : register(s0); 
SamplerState PointSampleClamp : register(s1); 


//--------------------------------------------------------------------------------------
// Shader code
//--------------------------------------------------------------------------------------
float4 main(LightingPixelShaderInput input) : SV_Target
{
    input.worldNormal = normalize(input.worldNormal);
    float3 cameraDirection = normalize(gCameraPositionR4 - input.worldPosition);

    // Light 1
    float3 light1Vector = gLightPulsatingPositionR4 - input.worldPosition;
    float light1Distance = length(light1Vector);
    float3 light1Direction = light1Vector / light1Distance; 
    
    float diffuseLevel1 = max(dot(input.worldNormal, light1Direction), 0);
    float cellDiffuseLevel1 = CellMap.Sample(PointSampleClamp, diffuseLevel1).r;
    float3 diffuseLight1 = gLightPulsatingColourR4 * cellDiffuseLevel1 / light1Distance;

    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR4); 
    

    // Light 2
    float3 light2Vector = gLightGradientPositionR4 - input.worldPosition;
    float light2Distance = length(light2Vector);
    float3 light2Direction = light2Vector / light2Distance;

    float diffuseLevel2 = max(dot(input.worldNormal, light2Direction), 0);
    float cellDiffuseLevel2 = CellMap.Sample(PointSampleClamp, diffuseLevel2).r;
    float3 diffuseLight2 = gLightGradientColourR4 * cellDiffuseLevel2 / light2Distance;

    halfway = normalize(light2Direction + cameraDirection);
    float3 specularLight2 = diffuseLight2 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR4);


    // Combine lighting with texture colours
    float4 textureColour = DiffuseMap.Sample(TexSampler, input.uv);
    float3 diffuseMaterialColour = textureColour.rgb;
    float specularMaterialColour = textureColour.a;

    float3 finalColour = (gAmbientColourR4 + diffuseLight1 + diffuseLight2) * diffuseMaterialColour +
                         (specularLight1 + specularLight2) * specularMaterialColour;

    return float4(finalColour, 1.0f); 
}