//--------------------------------------------------------------------------------------
// State creation
// - Sampler state (Bilinear, trilinear etc.)
// - Blender state (Additive blending, alpha blending etc.)
// - Rasterizer state (Wireframe mode, don't cull back faces etc.)
// - Depth stencil state (How to use the depth and stencil buffer)
//--------------------------------------------------------------------------------------
#ifndef _STATE_H_INCLUDED_
#define _STATE_H_INCLUDED_

#include "Direct3DSetup.h"
#include "Common.h"

// GPU "States" //

class StateManager
{
public:
	// Create all the states used in this app, returns true on success
	void ReleaseStates();
	// Release DirectX state objects
	bool CreateStates();
	
	StateManager(DirectXManager* dm) : dm(dm) {};

	ID3D11SamplerState* pointSampler;
	ID3D11SamplerState* trilinearSampler;
	ID3D11SamplerState* anisotropic4xSampler;
	ID3D11BlendState* noBlendingState;
	ID3D11BlendState* additiveBlendingState;
	ID3D11RasterizerState* cullBackState;
	ID3D11RasterizerState* cullFrontState;
	ID3D11RasterizerState* cullNoneState;
	ID3D11DepthStencilState* useDepthBufferState;
	ID3D11DepthStencilState* depthReadOnlyState;
	ID3D11DepthStencilState* noDepthBufferState;

private:
	DirectXManager* dm;
};
#endif //_STATE_H_INCLUDED_
