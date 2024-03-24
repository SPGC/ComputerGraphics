#pragma once
#include "../Camera.h"
#include "../Shader.h"
#include "../State.h"
#include "../Direct3DSetup.h"
class SceneSample
{
public:
	virtual bool InitGeometry() = 0;
	virtual bool InitScene() = 0;
	virtual void ReleaseResources() = 0;
	virtual void RenderScene() = 0;
	virtual void UpdateScene(float frameTime) = 0;
	virtual void ReSetScene() = 0;

	void SetShaderManager(ShadersManager* sm)
	{
		this->sm = sm;
	};
	void SetStateManager(StateManager* stateManager)
	{
		this->stateManager = stateManager;
	};
	void SetDirectXManager(DirectXManager* dm)
	{
		this->dm = dm;
	};
	void SetCamera(Camera* camera)
	{
		this->camera = camera;
	}

protected:
	ShadersManager* sm;
	DirectXManager* dm;
	StateManager* stateManager;
	Camera* camera;
};

