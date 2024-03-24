//--------------------------------------------------------------------------------------
// Scene geometry and layout preparation
// Scene rendering & update
//--------------------------------------------------------------------------------------

#ifndef _SCENE_H_INCLUDED_
#define _SCENE_H_INCLUDED_

#include "Light.h"
#include "Object.h"
#include "Camera.h"
#include "Shader.h"
#include "SceneSample.h"
#include "Requirment1.h"
#include "Requirment2.h"
#include "Requirment3.h"
#include "Requirment4.h"
#include "Direct3DSetup.h"

#include <map>
#include <vector>

//--------------------------------------------------------------------------------------
// Scene Geometry and Layout
//--------------------------------------------------------------------------------------

class Scene {
public:
	Scene(DirectXManager* dm) : dm(dm), sm(dm), stateManager(dm) {};
	bool InitGeometry();

	// Layout the scene
	// Returns true on success
	bool InitScene();

	// Release the geometry resources created above
	void ReleaseResources();


	//--------------------------------------------------------------------------------------
	// Scene Render and Update
	//--------------------------------------------------------------------------------------

	void RenderScene();

	// frameTime is the time passed since the last frame
	void UpdateScene(float frameTime);

	void SetDirectXManager(DirectXManager* dm);

	~Scene();

private:
	std::vector<SceneSample*> scenes = {
		new Requirment1(),
		new Requirment2(),
		new Requirment3(),
		new Requirment4()
	};
	Camera* camera;
	int currentState = 0;
	ShadersManager sm;
	StateManager stateManager;
	DirectXManager* dm;
};

#endif //_SCENE_H_INCLUDED_
