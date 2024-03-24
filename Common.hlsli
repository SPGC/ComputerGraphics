//--------------------------------------------------------------------------------------
// Common include file for all shaders
//--------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------
// Shader input / output
//--------------------------------------------------------------------------------------

struct BasicPixelShaderInput
{
    float4 projectedPosition : SV_Position;
};

struct BasicVertex
{
    float3 position : position;
    float3 normal : normal;
    float2 uv : uv;
};

struct NormalMappingPixelShaderInput
{
    float4 projectedPosition : SV_Position;     
    float3 worldPosition : worldPosition; 
    float3 modelNormal : modelNormal; 
    float3 modelTangent : modelTangent; 
    
    float2 uv : uv; 
};

struct TangentVertex
{
    float3 position : position;
    float3 normal : normal;
    float3 tangent : tangent;
    float2 uv : uv;
};

struct LightingPixelShaderInput
{
    float4 projectedPosition : SV_Position; 
    float3 worldPosition : worldPosition; 
    float3 worldNormal : worldNormal; 
    float2 uv : uv; 
};

struct SimplePixelShaderInput
{
    float4 projectedPosition : SV_Position;
    float2 uv : uv;
};


//--------------------------------------------------------------------------------------
// Constant Buffers
//--------------------------------------------------------------------------------------


cbuffer PerFrameGlobalConstants : register(b0) 
{
    float4x4 gViewMatrix;
    float4x4 gProjectionMatrix;
    float4x4 gViewProjectionMatrix;
}

cbuffer PerModelConstants : register(b1)
{
    float4x4 gWorldMatrix;
    float3 gObjectColour;
    float padding_0;
}


cbuffer PerFrameConstantsR1 : register(b2) 
{
    float4x4 gViewMatrixR1;
    float4x4 gProjectionMatrixR1;
    float4x4 gViewProjectionMatrixR1; 
    
    float3 gLightPulsatingPositionR1;
    float paddingR1_1;
    float3 gLightPulsatingColourR1;
    float paddingR1_2;

    float3 gLightGradientPositionR1;
    float paddingR1_3;
    float3 gLightGradientColourR1;
    float paddingR1_4;

    float3 gAmbientColourR1;
    float gSpecularPowerR1; 

    float3 gCameraPositionR1;
    float paddingR1_5;
}

cbuffer PerFrameConstantsR2 : register(b3)
{
    float4x4 gViewMatrixR2;
    float4x4 gProjectionMatrixR2;
    float4x4 gViewProjectionMatrixR2; 

    float3 gLightPositionR2;
    float gWiggleStateR2;
    float3 gLightColourR2;
    float gWiggleScaleR2;
    
    float3 gSphereCenterR2;
    float paddingR2_4;

    float3 gAmbientColourR2;
    float gSpecularPowerR2; 

    float3 gCameraPositionR2;
    float paddingR2_3;
};

cbuffer PerFrameConstantsR3 : register(b4)
{
    float4x4 gViewMatrixR3;
    float4x4 gProjectionMatrixR3;
    float4x4 gViewProjectionMatrixR3; 

    float3 gLightPositionR3;
    float gFadeStateR3;
    float3 gLightColourR3;
    float paddingR3_1;

    float3 gAmbientColourR3;
    float gSpecularPowerR3; 

    float3 gCameraPositionR3;
    float paddingR3_2;
};

cbuffer PerFrameConstantsR4 : register(b5) 
{
    float4x4 gViewMatrixR4;
    float4x4 gProjectionMatrixR4;
    float4x4 gViewProjectionMatrixR4; 
    
    float3 gLightPulsatingPositionR4;
    float paddingR4_1;
    float3 gLightPulsatingColourR4;
    float paddingR4_2;

    float3 gLightGradientPositionR4;
    float paddingR4_3;
    float3 gLightGradientColourR4;
    float paddingR4_4;

    float3 gAmbientColourR4;
    float gSpecularPowerR4; 

    float3 gCameraPositionR4;
    float paddingR4_5;
}
