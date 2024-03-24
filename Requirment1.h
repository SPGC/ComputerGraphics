#pragma once
#include "../SceneSample.h"
#include "../Common.h"
#include "../Light.h"
#include "../Object.h"
#include "../Camera.h"
#include "../Shader.h"

#include <map>


namespace R1 {
	enum ObjectsR1
	{
		Floor,
		Teapot
	};

	enum LightsR1
	{
		Pulsating,
		Gradient
	};
};

struct PerFrameConstantsR1
{
	CMatrix4x4 viewMatrixR1;
	CMatrix4x4 projectionMatrixR1;
	CMatrix4x4 viewProjectionMatrixR1; 

	CVector3   lightPulsatingPositionR1;
	float      paddingR1_1;
	CVector3   lightPulsatingColourR1;
	float      paddingR1_2;

	CVector3   lightGradientPositionR1;
	float      paddingR1_3;
	CVector3   lightGradientColourR1;
	float      paddingR1_4;

	CVector3   ambientColourR1;
	float      specularPowerR1;  

	CVector3   cameraPositionR1;
	float      paddingR1_5;
};

class Requirment1 : public SceneSample
{
public:
	bool InitGeometry() override;
	bool InitScene() override;
	void ReleaseResources() override;
	void RenderScene() override;
	void UpdateScene(float frameTime) override;
	void ReSetScene() override;
private:
	std::map<R1::ObjectsR1, Object> models;
	std::map<R1::LightsR1, Light> lights;

	PerFrameConstantsR1 perFrameConstants;
	ID3D11Buffer* perFrameConstantBuffer;

	PerFrameGlobalConstants perFrameGlobalConstants;
	ID3D11Buffer* perFrameGlobalConstantBuffer;

	PerModelConstants perModelConstants;     
	ID3D11Buffer* perModelConstantBuffer;

	CVector3 ambientColour = { 0.1f, 0.1f, 0.15f };
	float    specularPower = 64.0f; 

	const float lightChangingSpeed = 0.7f;
	const float initialPulsatingStrength = 30;
	float currentChange = 0;
	const CVector3 pulsatingColour = { 1.0f, 1.0f, 1.0f };
	const CVector3 initialGradientColor = { 1.0f, 0.0f, 0.0f };
	const CVector3 endGradientColor = { 0.0f, 1.0f, 1.0f };
	const CVector3 pulsatingLightStartPosition = { 30, 10,  0 };
	const CVector3 gradientLightStartPosition = { -10, 20, 10 };
	const CVector3 teapotInitialPosition = { 10, 10, 10 };
};

