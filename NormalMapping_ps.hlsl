//--------------------------------------------------------------------------------------
// Pixel shader for normal mapping models
//--------------------------------------------------------------------------------------

#include "Common.hlsli" // Shaders can also use include files - note the extension


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------
Texture2D DiffuseSpecularMap : register(t0);
Texture2D NormalMap : register(t1);
SamplerState TexSampler : register(s0); 

//--------------------------------------------------------------------------------------
// Shader code
//--------------------------------------------------------------------------------------
float4 main(NormalMappingPixelShaderInput input) : SV_Target
{
	// Normal Map Extraction

    
    float3 modelNormal = normalize(input.modelNormal);
    float3 modelTangent = normalize(input.modelTangent);
    float3 modelBiTangent = cross(modelNormal, modelTangent);
    float3x3 invTangentMatrix = float3x3(modelTangent, modelBiTangent, modelNormal);
    float3 textureNormal = 2.0f * NormalMap.Sample(TexSampler, input.uv).rgb - 1.0f; 
    float3 worldNormal = normalize(mul((float3x3) gWorldMatrix, mul(textureNormal, invTangentMatrix)));

    
	// Calculate lighting

    
    float3 cameraDirection = normalize(gCameraPositionR4 - input.worldPosition);

    // Light 1
    float3 light1Vector = gLightPulsatingPositionR4 - input.worldPosition;
    float light1Distance = length(light1Vector);
    float3 light1Direction = light1Vector / light1Distance; 
    float3 diffuseLight1 = gLightPulsatingColourR4 * max(dot(worldNormal, light1Direction), 0) / light1Distance;

    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(worldNormal, halfway), 0), gSpecularPowerR4);


    // Light 2
    float3 light2Vector = gLightGradientPositionR4 - input.worldPosition;
    float light2Distance = length(light2Vector);
    float3 light2Direction = light2Vector / light2Distance;
    float3 diffuseLight2 = gLightGradientColourR4 * max(dot(worldNormal, light2Direction), 0) / light2Distance;

    halfway = normalize(light2Direction + cameraDirection);
    float3 specularLight2 = diffuseLight2 * pow(max(dot(worldNormal, halfway), 0), gLightGradientColourR4);

    float4 textureColour = DiffuseSpecularMap.Sample(TexSampler, input.uv);
    float3 diffuseMaterialColour = textureColour.rgb;
    float specularMaterialColour = textureColour.a;

    // Combine lighting with texture colours
    float3 finalColour = (gAmbientColourR4 + diffuseLight1 + diffuseLight2) * diffuseMaterialColour +
                         (specularLight1 + specularLight2) * specularMaterialColour;

    return float4(finalColour, 1.0f); 
}