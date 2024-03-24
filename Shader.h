//--------------------------------------------------------------------------------------
// Loading GPU shaders
// Creation of constant buffers to help send C++ values to shaders each frame
//--------------------------------------------------------------------------------------
#ifndef _SHADER_H_INCLUDED_
#define _SHADER_H_INCLUDED_

#include "Common.h"
#include "Direct3DSetup.h"

#include <map>
#include <string>

class ShadersManager {
public:
	static ID3DBlob* CreateSignatureForVertexLayout(const D3D11_INPUT_ELEMENT_DESC vertexLayout[], int numElements);
	static ID3D11Buffer* CreateConstantBuffer(int size, DirectXManager* dm);
	ShadersManager(DirectXManager* dm) : dm(dm) {};
	void ReleaseShaders();
	bool LoadShaders();
	std::map<std::string, ID3D11VertexShader*> vsMap;
	std::map<std::string, ID3D11PixelShader*> psMap;
private:
	DirectXManager* dm;
	ID3D11VertexShader* LoadVertexShader(std::string shaderName);
	ID3D11PixelShader* LoadPixelShader(std::string shaderName);
};

#endif //_SHADER_H_INCLUDED_
