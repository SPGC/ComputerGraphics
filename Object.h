#pragma once
#include "Mesh.h"
#include "Model.h"
#include "Common.h"
#include "GraphicsHelpers.h"
#include "Direct3DSetup.h"

#include <string>
#include <assert.h>
#include <vector>

class Object
{
private:
	Mesh* mesh;
	Model* model;
	std::vector<ID3D11Resource*> texture;
	std::vector<ID3D11ShaderResourceView*> textureGPU;
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;
	ID3D11SamplerState* sampler;
	std::string name;
	bool needTangent;
	DirectXManager* dm;
public:
	Object();
	Object(std::string name, DirectXManager* dm);
	Object(std::string name, DirectXManager* dm, bool needTangent);
	bool initMesh(std::string file);
	void initModel();
	bool initTextures(std::string name, int index);
	void setPS(ID3D11PixelShader* pixelShader);
	void setVS(ID3D11VertexShader* vertexShader);
	void setSampler(ID3D11SamplerState* sampler);
	Model* getModel();
	ID3D11Resource* getTexture(int index);
	ID3D11ShaderResourceView* getGPUTexture(int index);
	void render(PerModelConstants perModelConstants, ID3D11Buffer* perModelConstantBuffer);
	void release();
};

