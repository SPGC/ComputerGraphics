#ifndef _DIRECT3D_SETUP_H_INCLUDED_
#define _DIRECT3D_SETUP_H_INCLUDED_

#include <d3d11.h>

//--------------------------------------------------------------------------------------
// Initialisation of Direct3D and main resources
//--------------------------------------------------------------------------------------

class DirectXManager
{
public:
	// Returns false on failure
	bool InitDirect3D();

	// Release the memory held by all objects created
	void ShutdownDirect3D();

	// The main Direct3D (D3D) variables
	ID3D11Device* mainD3DDevice = nullptr; // D3D device for overall features
	ID3D11DeviceContext* mainD3DContext = nullptr; // D3D context for specific rendering tasks

	// Swap chain and back buffer
	IDXGISwapChain* swapChain = nullptr;
	ID3D11RenderTargetView* backBufferRenderTarget = nullptr;

	// Depth buffer (can also contain "stencil" values, which we will see later)
	ID3D11Texture2D* depthStencilTexture = nullptr; // The texture holding the depth values
	ID3D11DepthStencilView* depthStencil = nullptr; // The depth buffer referencing above texture

};
#endif //_DIRECT3D_SETUP_H_INCLUDED_
