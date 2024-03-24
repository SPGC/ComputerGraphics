#include "Object.h"
#include <stdexcept>

Object::Object(std::string name, DirectXManager* dm)
{
	this->name = name;
	needTangent = false;
	this->dm = dm;
}
Object::Object(std::string name, DirectXManager* dm, bool needTangent)
{
	this->name = name;
	this->needTangent = needTangent;
	this->dm = dm;
}
bool Object::initMesh(std::string file)
{
	try 
	{
		mesh = new Mesh(file, dm, needTangent);
	} 
	catch (std::runtime_error e)
	{
		gLastError = e.what(); 
		return false;
	}
}
void Object::initModel()
{
	model = new Model(mesh, dm);
}
bool Object::initTextures(std::string name, int index) {
	assert(index <= texture.size());
	if (index == texture.size()) 
	{
		texture.push_back(nullptr);
		textureGPU.push_back(nullptr);
		if (!LoadTexture(dm, name, &texture[index], &textureGPU[index])) {
			gLastError = "Error in loading texture for " + name;
			return false;
		}
		return true;
	} 
	else
	{
		if (!LoadTexture(dm, name, &texture[index], &textureGPU[index])) {
			gLastError = "Error in loading texture for " + name;
			return false;
		}
		return true;
	}
	
}
void Object::setPS(ID3D11PixelShader* pixelShader)
{
	this->pixelShader = pixelShader;
}
void Object::setVS(ID3D11VertexShader* vertexShader)
{
	this->vertexShader = vertexShader;
}
void Object::setSampler(ID3D11SamplerState* sampler)
{
	this->sampler = sampler;
}
Model* Object::getModel()
{
	return model;
}
ID3D11Resource* Object::getTexture(int index)
{
	assert(index < texture.size());
	return texture[index];
}
ID3D11ShaderResourceView* Object::getGPUTexture(int index)
{
	assert(index < texture.size());
	return textureGPU[index];
}
void Object::render(PerModelConstants perModelConstants, ID3D11Buffer* perModelConstantBuffer)
{
	for (int i = 0; i < textureGPU.size(); ++i)
	{
		dm->mainD3DContext->PSSetShaderResources(i, 1, &textureGPU[i]); // First parameter must match texture slot number in the shaer
	}
	
	dm->mainD3DContext->PSSetSamplers(0, 1, &sampler);

	// Render model, sets world matrix, vertex and index buffer and calls Draw on the GPU
	model->Render(perModelConstants, perModelConstantBuffer);
}
Object::Object()
{
	name = "Uninitialized";
}

void Object::release() {
	for (int i = 0; i < texture.size(); ++i) {
		if (texture[i]) {
			texture[i]->Release();
		}
		if (textureGPU[i]) {
			textureGPU[i]->Release();
		}
	}
	if (model)
	{
		delete model;
		model = nullptr;
	}
	if (mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}