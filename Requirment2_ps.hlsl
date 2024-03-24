//--------------------------------------------------------------------------------------
// Pixel Shader for Requirmenet2
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
    float3 cameraDirection = normalize(gCameraPositionR2 - input.worldPosition);

    // Light 
    float distance1 = distance(gLightPositionR2, input.worldPosition);
    float3 light1Direction = normalize(gLightPositionR2 - input.worldPosition);
    float3 diffuseLight1 = gLightColourR2 * max(dot(input.worldNormal, light1Direction), 0) / distance1;

    float3 halfway = normalize(light1Direction + cameraDirection);
    float3 specularLight1 = diffuseLight1 * pow(max(dot(input.worldNormal, halfway), 0), gSpecularPowerR2) / distance1; 


    // Combine lighting with texture colours
    float4 textureColour = DiffuseSpecularMap.Sample(TexSampler, float2(input.uv.x + cos(input.uv.y * gWiggleStateR2) * 0.1, input.uv.y + cos(input.uv.x * gWiggleStateR2) * 0.1));
    float3 diffuseMaterialColour = textureColour.rgb;
    float specularMaterialColour = 1;

    float3 finalColour = (gAmbientColourR2 + diffuseLight1) * diffuseMaterialColour +
                         (specularLight1) * specularMaterialColour;
    
    return float4(finalColour, 1.0f);
}