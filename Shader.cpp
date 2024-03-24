//--------------------------------------------------------------------------------------
// Loading GPU shaders
// Creation of constant buffers to help send C++ values to shaders each frame
//--------------------------------------------------------------------------------------

#include "Shader.h"
#include <fstream>
#include <vector>
#include <d3dcompiler.h>


//--------------------------------------------------------------------------------------
// Shader creation / destruction
//--------------------------------------------------------------------------------------


ID3DBlob* ShadersManager::CreateSignatureForVertexLayout(const D3D11_INPUT_ELEMENT_DESC vertexLayout[], int numElements)
{
    std::string shaderSource = "float4 main(";
    for (int elt = 0; elt < numElements; ++elt)
    {
        auto& format = vertexLayout[elt].Format;
        // This list should be more complete for production use
        if (format == DXGI_FORMAT_R32G32B32A32_FLOAT) shaderSource += "float4";
        else if (format == DXGI_FORMAT_R32G32B32_FLOAT)    shaderSource += "float3";
        else if (format == DXGI_FORMAT_R32G32_FLOAT)       shaderSource += "float2";
        else if (format == DXGI_FORMAT_R32_FLOAT)          shaderSource += "float";
        else return nullptr; // Unsupported type in layout

        uint8_t index = static_cast<uint8_t>(vertexLayout[elt].SemanticIndex);
        std::string semanticName = vertexLayout[elt].SemanticName;
        semanticName += ('0' + index);

        shaderSource += " ";
        shaderSource += semanticName;
        shaderSource += " : ";
        shaderSource += semanticName;
        if (elt != numElements - 1)  shaderSource += " , ";
    }
    shaderSource += ") : SV_Position {return 0;}";

    ID3DBlob* compiledShader;
    HRESULT hr = D3DCompile(shaderSource.c_str(), shaderSource.length(), NULL, NULL, NULL, "main",
        "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL0, 0, &compiledShader, NULL);
    if (FAILED(hr))
    {
        return nullptr;
    }

    return compiledShader;
}
ID3D11Buffer* ShadersManager::CreateConstantBuffer(int size, DirectXManager* dm)
{
    D3D11_BUFFER_DESC cbDesc;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.ByteWidth = 16 * ((size + 15) / 16);     // Constant buffer size must be a multiple of 16 - this maths rounds up to the nearest multiple
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;             // Indicates that the buffer is frequently updated
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU is only going to write to the constants (not read them)
    cbDesc.MiscFlags = 0;
    ID3D11Buffer* constantBuffer;
    HRESULT hr = dm->mainD3DDevice->CreateBuffer(&cbDesc, nullptr, &constantBuffer);
    if (FAILED(hr))
    {
        return nullptr;
    }

    return constantBuffer;
}
void ShadersManager::ReleaseShaders()
{
    for (auto it = vsMap.begin(); it != vsMap.end(); ++it)
    {
        if (it->second) 
        {
            it->second->Release();
        }
    }

    for (auto it = psMap.begin(); it != psMap.end(); ++it)
    {
        if (it->second)
        {
            it->second->Release();
        }
    }
}
bool ShadersManager::LoadShaders()
{
    vsMap.insert(std::make_pair("GeneralModelCommon_vs", LoadVertexShader("GeneralModelCommon_vs")));
    vsMap.insert(std::make_pair("LightModelsCommon_vs", LoadVertexShader("LightModelsCommon_vs")));
    psMap.insert(std::make_pair("LightModelsCommon_ps", LoadPixelShader("LightModelsCommon_ps")));
    psMap.insert(std::make_pair("Requirment1_ps", LoadPixelShader("Requirment1_ps")));
    psMap.insert(std::make_pair("Requirment2_ps", LoadPixelShader("Requirment2_ps")));
    vsMap.insert(std::make_pair("Requirment2_vs", LoadVertexShader("Requirment2_vs")));
    psMap.insert(std::make_pair("Requirment3_ps", LoadPixelShader("Requirment3_ps")));
    psMap.insert(std::make_pair("Portal_ps", LoadPixelShader("Portal_ps")));
    psMap.insert(std::make_pair("Mirror_ps", LoadPixelShader("Mirror_ps")));
    psMap.insert(std::make_pair("CommonPixelShader_ps", LoadPixelShader("CommonPixelShader_ps")));
    vsMap.insert(std::make_pair("ParalaxMapping_vs", LoadVertexShader("ParalaxMapping_vs")));
    psMap.insert(std::make_pair("ParalaxMapping_ps", LoadPixelShader("ParalaxMapping_ps")));
    psMap.insert(std::make_pair("NormalMapping_ps", LoadPixelShader("NormalMapping_ps")));
    psMap.insert(std::make_pair("CartoonModel_ps", LoadPixelShader("CartoonModel_ps")));
    vsMap.insert(std::make_pair("CartoonModelOutline_vs", LoadVertexShader("CartoonModelOutline_vs")));
    psMap.insert(std::make_pair("CartoonModelOutline_ps", LoadPixelShader("CartoonModelOutline_ps")));

    

    for (auto it = vsMap.begin(); it != vsMap.end(); ++it)
    {
        if (!it->second)
        {
            gLastError = "Error loading shader" + it->first;
            return false;
        }
    }

    for (auto it = psMap.begin(); it != psMap.end(); ++it)
    {
        if (!it->second)
        {
            gLastError = "Error loading shader" + it->first;
            return false;
        }
    }

    return true;
}
ID3D11VertexShader* ShadersManager::LoadVertexShader(std::string shaderName)
{
    // Open compiled shader object file
    std::ifstream shaderFile(shaderName + ".cso", std::ios::in | std::ios::binary | std::ios::ate);
    if (!shaderFile.is_open())
    {
        return nullptr;
    }

    // Read file into vector of chars
    std::streamoff fileSize = shaderFile.tellg();
    shaderFile.seekg(0, std::ios::beg);
    std::vector<char> byteCode(fileSize);
    shaderFile.read(&byteCode[0], fileSize);
    if (shaderFile.fail())
    {
        return nullptr;
    }

    // Create shader object from loaded file (we will use the object later when rendering)
    ID3D11VertexShader* shader;
    HRESULT hr = dm->mainD3DDevice->CreateVertexShader(byteCode.data(), byteCode.size(), nullptr, &shader);
    if (FAILED(hr))
    {
        return nullptr;
    }

    return shader;
}
ID3D11PixelShader* ShadersManager::LoadPixelShader(std::string shaderName)
{
    // Open compiled shader object file
    std::ifstream shaderFile(shaderName + ".cso", std::ios::in | std::ios::binary | std::ios::ate);
    if (!shaderFile.is_open())
    {
        return nullptr;
    }

    // Read file into vector of chars
    std::streamoff fileSize = shaderFile.tellg();
    shaderFile.seekg(0, std::ios::beg);
    std::vector<char>byteCode(fileSize);
    shaderFile.read(&byteCode[0], fileSize);
    if (shaderFile.fail())
    {
        return nullptr;
    }

    // Create shader object from loaded file (we will use the object later when rendering)
    ID3D11PixelShader* shader;
    HRESULT hr = dm->mainD3DDevice->CreatePixelShader(byteCode.data(), byteCode.size(), nullptr, &shader);
    if (FAILED(hr))
    {
        return nullptr;
    }

    return shader;
}
