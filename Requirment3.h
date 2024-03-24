#pragma once
#include "../SceneSample.h"
#include "../Common.h"
#include "../Light.h"
#include "../Object.h"
#include "../Camera.h"
#include "../Shader.h"

#include <map>

namespace R3 {
	enum ObjectsR3
	{
		Cube
	};

	enum LightsR3
	{
		MainLight
	};
};

struct PerFrameConstantsR3
{
	CMatrix4x4 viewMatrixR3;
	CMatrix4x4 projectionMatrixR3;
	CMatrix4x4 viewProjectionMatrixR3;

	CVector3   lightPositionR3;
	float      fadingStateR3;
	CVector3   lightColourR3;
	float      paddingR3_1;

	CVector3   ambientColourR3;
	float      specularPowerR3; 

	CVector3   cameraPositionR3;
	float      paddingR3_2;
};

class Requirment3 : public SceneSample
{
public:
	bool InitGeometry() override;
	bool InitScene() override;
	void ReleaseResources() override;
	void RenderScene() override;
	void UpdateScene(float frameTime) override;
	void ReSetScene() override;

private:
	std::map<R3::ObjectsR3, Object> models;
	std::map<R3::LightsR3, Light> lights;

	PerFrameConstantsR3 perFrameConstants;
	ID3D11Buffer* perFrameConstantBuffer;

	PerFrameGlobalConstants perFrameGlobalConstants;
	ID3D11Buffer* perFrameGlobalConstantBuffer;

	PerModelConstants perModelConstants;     
	ID3D11Buffer* perModelConstantBuffer;

	CVector3 ambientColour = { 0.1f, 0.1f, 0.15f };
	float    specularPower = 64.0f; 

	const CVector3 lightColour = { 1.0f, 1.0f, 1.0f };
	const float lightStrength = 10.0;
	const CVector3 lightPosition = { -10, 20, 10 };
	const CVector3 cubePosition = { 10, 10, 10 };
	float fadingState = 0;
	const float fadingSpeed = 2.0;
};

