#pragma once
#include "../SceneSample.h"
#include "../Common.h"
#include "../Light.h"
#include "../Object.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../State.h"

#include <map>

namespace R4 {
	enum ObjectsR4
	{
		Floor,
		NormalMappingModel,
		ParalaxMappingModel,
		TV,
		Mirror,
		Shrek
	};

	enum LightsR4
	{
		Pulsating,
		Gradient
	};
};

struct PerFrameConstantsR4
{
	CMatrix4x4 viewMatrix;
	CMatrix4x4 projectionMatrix;
	CMatrix4x4 viewProjectionMatrix; 

	CVector3   lightPulsatingPosition;
	float      padding1;
	CVector3   lightPulsatingColour;
	float      padding2;

	CVector3   lightGradientPosition;
	float      padding3;
	CVector3   lightGradientColour;
	float      padding4;

	CVector3   ambientColour;
	float      specularPower;  

	CVector3   cameraPosition;
	float      padding5;
};


class Requirment4 : public SceneSample
{
public:
	bool InitGeometry() override;
	bool InitScene() override;
	void ReleaseResources() override;
	void RenderScene() override;
	void UpdateScene(float frameTime) override;
	void ReSetScene() override;
private:
	std::map<R4::ObjectsR4, Object> models;
	std::map<R4::LightsR4, Light> lights;
	void RenderCameraScene(Camera* camera);

	PerFrameConstantsR4 perFrameConstants;
	ID3D11Buffer* perFrameConstantBuffer;

	PerFrameGlobalConstants perFrameGlobalConstants;
	ID3D11Buffer* perFrameGlobalConstantBuffer;

	PerModelConstants perModelConstants;      
	ID3D11Buffer* perModelConstantBuffer;

	CVector3 ambientColour = { 0.3f, 0.3f, 0.45f }; 
	float    specularPower = 64.0f; 

	Camera* portalCamera;
	Camera* mirrorCamera;
	const float lightChangingSpeed = 0.7f;
	const float initialPulsatingStrength = 30;
	float currentChange = 0;
	const CVector3 pulsatingColour = { 1.0f, 1.0f, 1.0f };
	const CVector3 initialGradientColor = { 1.0f, 0.0f, 0.0f };
	const CVector3 endGradientColor = { 0.0f, 1.0f, 1.0f };
	const CVector3 pulsatingLightStartPosition = { 30, 10,  0 };
	const CVector3 gradientLightStartPosition = { -10, 20, 10 };
	const CVector3 NormalMappingModelInitialPosition = { 20, 30, 10 };
	const CVector3 ParalaxMappingModelInitialPosition = { 20, 10, 10 };
	const CVector3 PortalInitialPosition = { 30, 10, -30 };
	const CVector3 MirrorInitialPosition = { 30, 10, 60 };
	const CVector3 PortalCameraInitialPosition = { -33.98, 13.58, 38.2 };
	const CVector3 MirrorCameraInitialPosition = { 40, 10, 10 };
	const CVector3 PortalCameraInitialRoatation = { -0.14, 2.31, 0.0};
	const CVector3 MirrorInitialRoatation = { 0.0,PI,0.0 };
	const CVector3 ShrekInitialPosition = { 0.0, 0.0, 0.0 };
	const float ShrekInitialScale = 10;
	const float portalWidth = 1024;
	const float portalHeight = 1024;

	// Portal amd mirro textures, depth and stencil buffers

	ID3D11Texture2D* portalTexture = nullptr;
	ID3D11RenderTargetView* portalRenderTarget = nullptr;
	ID3D11ShaderResourceView* portalTextureSRV = nullptr;

	ID3D11Texture2D* portalDepthStencil = nullptr; 
	ID3D11DepthStencilView* portalDepthStencilView = nullptr; 

	ID3D11Texture2D* mirrorTexture = nullptr;
	ID3D11RenderTargetView* mirrorRenderTarget = nullptr;
	ID3D11ShaderResourceView* mirrorTextureSRV = nullptr;
	
	ID3D11Texture2D* mirrorDepthStencil = nullptr; 
	ID3D11DepthStencilView* mirrorDepthStencilView = nullptr; 
};

