#pragma once
#include "../SceneSample.h"
#include "../Common.h"
#include "../Light.h"
#include "../Object.h"
#include "../Camera.h"
#include "../Shader.h"

#include <map>

namespace R2 {
	enum ObjectsR2
	{
		Sphere
	};

	enum LightsR2
	{
		MainLight
	};
};

struct PerFrameConstantsR2
{
	CMatrix4x4 viewMatrixR2;
	CMatrix4x4 projectionMatrixR2;
	CMatrix4x4 viewProjectionMatrixR2; // The above two matrices multiplied together to combine their effects

	CVector3   lightPositionR2;
	float      wiggleStateR2;
	CVector3   lightColourR2;
	float      wiggleScaleR2;

	CVector3   sphereCenterR2;
	float      paddingR2_4;

	CVector3   ambientColourR2;
	float      specularPowerR2;  // In this case we actually have a useful float variable that we can use to pad to a float4

	CVector3   cameraPositionR2;
	float      paddingR2_3;
};

class Requirment2 : public SceneSample
{
public:
	bool InitGeometry() override;
	bool InitScene() override;
	void ReleaseResources() override;
	void RenderScene() override;
	void UpdateScene(float frameTime) override;
	void ReSetScene() override;

private:
	std::map<R2::ObjectsR2, Object> models;
	std::map<R2::LightsR2, Light> lights;

	PerFrameConstantsR2 perFrameConstants;
	ID3D11Buffer* perFrameConstantBuffer;

	PerFrameGlobalConstants perFrameGlobalConstants;
	ID3D11Buffer* perFrameGlobalConstantBuffer;

	PerModelConstants perModelConstants;
	ID3D11Buffer* perModelConstantBuffer;

	CVector3 ambientColour = { 0.1f, 0.1f, 0.15f }; 
	float    specularPower = 64.0f; 

	const CVector3 lightColour = { 1.0f, 1.0f, 1.0f };
	const float lightStrength = 40.0;
	const CVector3 lightPosition = { -10, 20, 10 };
	const CVector3 spherePosition = { 10, 10, 10 };
	const float scalingFactor = 0.1;
	float wiggleState = 0;
	const float wiggleSpeed = 2.0;
};

