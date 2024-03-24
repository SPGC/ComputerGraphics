//--------------------------------------------------------------------------------------
// Scene geometry and layout preparation
// Scene rendering & update
//--------------------------------------------------------------------------------------

#include "Scene.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"
#include "State.h"
#include "Input.h"
#include "Common.h"

#include "CVector2.h" 
#include "CVector3.h" 
#include "CMatrix4x4.h"
#include "MathHelpers.h"     // Helper functions for maths
#include "GraphicsHelpers.h" // Helper functions to unclutter the code here

#include "ColourRGBA.h" 

#include <sstream>
#include <memory>



//--------------------------------------------------------------------------------------
// Initialise scene geometry, constant buffers and states
//--------------------------------------------------------------------------------------

// Prepare the geometry required for the scene
// Returns true on success
bool Scene::InitGeometry()
{
    // Load the shaders required for the geometry we will use (see Shader.cpp / .h)
    if (!sm.LoadShaders())
    {
        gLastError = "Error loading shaders";
        return false;
    }    

	// Create all filtering modes, blending modes etc. used by the app (see State.cpp/.h)

    for (int i = 0; i < scenes.size(); ++i)
    {
        scenes[i]->SetDirectXManager(dm);
        if (!scenes[i]->InitGeometry())
        {
            return false;
        }
        scenes[i]->SetShaderManager(&sm);
        scenes[i]->SetStateManager(&stateManager);
    }

	if (!stateManager.CreateStates())
	{
		gLastError = "Error creating states";
		return false;
	}

	return true;
}


// Prepare the scene
// Returns true on success
bool Scene::InitScene()
{
    camera = new Camera();
    camera->SetPosition({ -15, 20, -40 });
    camera->SetRotation({ 0, 0, 0.0f });

    for (int i = 0; i < scenes.size(); ++i)
    {
        scenes[i]->SetCamera(camera);
        if (!scenes[i]->InitScene())
        {
            return false;
        }
    }

    return true;
}


// Release the geometry and scene resources created above
void Scene::ReleaseResources()
{
    stateManager.ReleaseStates();

    sm.ReleaseShaders();
    for (int i = 0; i < scenes.size(); ++i)
    {
        scenes[i]->ReleaseResources();
    }
}



//--------------------------------------------------------------------------------------
// Scene Rendering
//--------------------------------------------------------------------------------------

// Called once a frame, from the loop in Main.cpp
void Scene::RenderScene()
{
    //// Per-frame set-up ////

    // Set the "back buffer" as the target for rendering. The "back buffer" is an off-screen viewport. When
    // we have finished drawing to the back buffer it sent to the "front buffer" - which is the monitor.
    dm->mainD3DContext->OMSetRenderTargets(1, &dm->backBufferRenderTarget, dm->depthStencil);
    dm->mainD3DContext->ClearDepthStencilView(dm->depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Clear the back buffer to a fixed colour
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red, green, blue, alpha
    dm->mainD3DContext->ClearRenderTargetView(dm->backBufferRenderTarget, ClearColor);

    // Setup the viewport - defines which part of the window we will render to, almost always the whole window
    D3D11_VIEWPORT vp;
    vp.Width  = static_cast<FLOAT>(gViewportWidth);
    vp.Height = static_cast<FLOAT>(gViewportHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    dm->mainD3DContext->RSSetViewports(1, &vp);

    scenes[currentState]->RenderScene();

    // When drawing to the off-screen back buffer is complete, we "present" the image to the front buffer (the screen)
    dm->swapChain->Present(0, 0);
}


//--------------------------------------------------------------------------------------
// Scene Update
//--------------------------------------------------------------------------------------

// Update models and camera. frameTime is the time passed since the last frame
void Scene::UpdateScene(float frameTime)
{
    for (int i = 0; i < scenes.size(); ++i)
    {
        scenes[i]->UpdateScene(frameTime);
    }
	// Control camera (will update its view matrix)
	camera->Control( frameTime, Key_Up, Key_Down, Key_Left, Key_Right, Key_W, Key_S, Key_A, Key_D );

    if (KeyHit(Key_1))
    {
        currentState = 0;
    }
    if (KeyHit(Key_2))
    {
        currentState = 1;
    }
    if (KeyHit(Key_3))
    {
        currentState = 2;
    }
    if (KeyHit(Key_4))
    {
        currentState = 3;
    }
    // Show frame time / FPS in the window title //
    const float fpsUpdateTime = 0.5f; // How long between updates (in seconds)
    static float totalFrameTime = 0;
    static int frameCount = 0;
    totalFrameTime += frameTime;
    ++frameCount;
    if (totalFrameTime > fpsUpdateTime)
    {
        // Displays FPS rounded to nearest int, and frame time (more useful for developers) in milliseconds to 2 decimal places
        float avgFrameTime = totalFrameTime / frameCount;
        std::ostringstream frameTimeMs;
        frameTimeMs.precision(2);
        frameTimeMs << std::fixed << avgFrameTime * 1000;
        std::string windowTitle = "CO2409 Week 16: Pixel Lighting 2 - Frame Time: " + frameTimeMs.str() +
                                  "ms, FPS: " + std::to_string(static_cast<int>(1 / avgFrameTime + 0.5f));
        SetWindowTextA(gHWnd, windowTitle.c_str());
        totalFrameTime = 0;
        frameCount = 0;
    }
}

void Scene::SetDirectXManager(DirectXManager* dm)
{
    this->dm = dm;
}

Scene::~Scene()
{
    for (int i = 0; i < scenes.size(); ++i)
    {
        delete scenes[i];
    }
}
