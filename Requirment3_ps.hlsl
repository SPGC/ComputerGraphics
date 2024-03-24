//--------------------------------------------------------------------------------------
// Pixel Shader for Requirmenet3
//--------------------------------------------------------------------------------------

#include "Common.hlsli"


//--------------------------------------------------------------------------------------
// Textures (texture maps)
//--------------------------------------------------------------------------------------

Texture2D DiffuseSpecularMap : register(t0); 
Texture2D DiffuseSpecularMap2 : register(t1);  
SamplerState TexSampler : register(s0);


//--------------------------------------------------------------------------------------
// Shader code
//--------------------------------------------------------------------------------------
float4 main(LightingPixelShaderInput input) : SV_Target
{
    input.worldNormal = normalize(input.worldNormal); 
    float3 cameraDirection = normalize(gCameraPositionR3 - input.worldPosition);

    // Light 
    float distance1 = distance(gLightPositionR3, input.worldPosition);
    float3 light1Direction = normalize(gLightPositionR3 - input.worldPosition);
    float3 diffuseLight1 = gLightColourR3 * max(dot(input.worldNormal, light1Direction), 0) / distance1;

    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR3) / distance1; // Multiplying by diffuseLight instead of light colour - my own personal preference

    // Combine lighting with texture colours
    float4 textureColour1 = DiffuseSpecularMap.Sample(TexSampler, input.uv);
    float4 textureColour2 = DiffuseSpecularMap2.Sample(TexSampler, input.uv);
    
    float3 diffuseMaterialColour1 = textureColour1.rgb;
    float specularMaterialColour1 = textureColour1.a;
    
    float3 diffuseMaterialColour2 = textureColour2.rgb;
    float specularMaterialColour2 = textureColour2.a;
    
    float3 diffuseMaterialColour = lerp(diffuseMaterialColour1, diffuseMaterialColour2, (cos(gFadeStateR3) + 1) / 2);
    float specularMaterialColour = lerp(specularMaterialColour1, specularMaterialColour2, (cos(gFadeStateR3) + 1) / 2);

    float3 finalColour = (gAmbientColourR3 + diffuseLight1) * diffuseMaterialColour +
                         specularLight1 * specularMaterialColour;
    
    return float4(finalColour, 1.0f); 
}