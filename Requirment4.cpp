#include "Requirment4.h"
#include "../State.h"
#include "../Matrix.h"
#include <vector>

using namespace R4;

bool Requirment4::InitGeometry()
{
    models.insert(std::make_pair(Floor, Object("floor", dm)));
    models.insert(std::make_pair(NormalMappingModel, Object("normal_mapping_model", dm, true)));
    models.insert(std::make_pair(ParalaxMappingModel, Object("paralax_mapping_model", dm, true)));
    models.insert(std::make_pair(Shrek, Object("Shrek", dm)));
    models.insert(std::make_pair(TV, Object("tv", dm)));
    models.insert(std::make_pair(Mirror, Object("mirror", dm)));
    lights.insert(std::make_pair(Pulsating, Light("pulsating", dm, pulsatingColour, initialPulsatingStrength)));
    lights.insert(std::make_pair(Gradient, Light("gradient", dm, initialGradientColor, initialPulsatingStrength)));

    if (!(
        models[Floor].initMesh("floor.x")
        && models[NormalMappingModel].initMesh("teapot.x")
        && models[ParalaxMappingModel].initMesh("cube.x")
        && models[Shrek].initMesh("troll.x")
        && models[TV].initMesh("cube.x")
        && models[Mirror].initMesh("portal.x")
        && lights[Pulsating].initMesh("light.x")
        && lights[Gradient].initMesh("light.x")
        ))
    {
        return false;
    }

    perFrameConstantBuffer = ShadersManager::CreateConstantBuffer(sizeof(perFrameConstants), dm);
    perModelConstantBuffer = ShadersManager::CreateConstantBuffer(sizeof(perModelConstants), dm);
    perFrameGlobalConstantBuffer = ShadersManager::CreateConstantBuffer(sizeof(perFrameGlobalConstants), dm);
    if (perFrameConstantBuffer == nullptr || perModelConstantBuffer == nullptr || perFrameGlobalConstantBuffer == nullptr)
    {
        gLastError = "Error creating constant buffers";
        return false;
    }
    
    if (!(
        models[Floor].initTextures("WoodDiffuseSpecular.dds", 0)
        && models[NormalMappingModel].initTextures("PatternDiffuseSpecular.dds", 0)
        && models[NormalMappingModel].initTextures("PatternNormal.dds", 1)
        && models[ParalaxMappingModel].initTextures("TechDiffuseSpecular.dds", 0)
        && models[ParalaxMappingModel].initTextures("TechNormalHeight.dds", 1)
        && models[Shrek].initTextures("Green.png", 0)
        && models[Shrek].initTextures("CellGradient.png", 1)
        && models[TV].initTextures("tv.dds", 0)
        && lights[Pulsating].initTextures("Flare.jpg", 0)
        && lights[Gradient].initTextures("Flare.jpg", 0)
        ))
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC portalDesc = {};
    portalDesc.Width = portalWidth;  
    portalDesc.Height = portalHeight;
    portalDesc.MipLevels = 1; 
    portalDesc.ArraySize = 1;
    portalDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    portalDesc.SampleDesc.Count = 1;
    portalDesc.SampleDesc.Quality = 0;
    portalDesc.Usage = D3D11_USAGE_DEFAULT;
    portalDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE; 
    portalDesc.CPUAccessFlags = 0;
    portalDesc.MiscFlags = 0;
    if (FAILED(dm->mainD3DDevice->CreateTexture2D(&portalDesc, NULL, &portalTexture)))
    {
        gLastError = "Error creating portal texture";
        return false;
    }

    if (FAILED(dm->mainD3DDevice->CreateRenderTargetView(portalTexture, NULL, &portalRenderTarget)))
    {
        gLastError = "Error creating portal render target view";
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc = {};
    srDesc.Format = portalDesc.Format;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MostDetailedMip = 0;
    srDesc.Texture2D.MipLevels = 1;
    if (FAILED(dm->mainD3DDevice->CreateShaderResourceView(portalTexture, &srDesc, &portalTextureSRV)))
    {
        gLastError = "Error creating portal shader resource view";
        return false;
    }

    D3D11_TEXTURE2D_DESC mirrorDesc = {};
    mirrorDesc.Width = portalWidth;  
    mirrorDesc.Height = portalHeight;
    mirrorDesc.MipLevels = 1;
    mirrorDesc.ArraySize = 1;
    mirrorDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
    mirrorDesc.SampleDesc.Count = 1;
    mirrorDesc.SampleDesc.Quality = 0;
    mirrorDesc.Usage = D3D11_USAGE_DEFAULT;
    mirrorDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE; 
    mirrorDesc.CPUAccessFlags = 0;
    mirrorDesc.MiscFlags = 0;
    if (FAILED(dm->mainD3DDevice->CreateTexture2D(&mirrorDesc, NULL, &mirrorTexture)))
    {
        gLastError = "Error creating portal texture";
        return false;
    }

    if (FAILED(dm->mainD3DDevice->CreateRenderTargetView(mirrorTexture, NULL, &mirrorRenderTarget)))
    {
        gLastError = "Error creating portal render target view";
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc2 = {};
    srDesc2.Format = portalDesc.Format;
    srDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc2.Texture2D.MostDetailedMip = 0;
    srDesc2.Texture2D.MipLevels = 1;
    if (FAILED(dm->mainD3DDevice->CreateShaderResourceView(mirrorTexture, &srDesc, &mirrorTextureSRV)))
    {
        gLastError = "Error creating portal shader resource view";
        return false;
    }


    portalDesc = {};
    portalDesc.Width = portalWidth;
    portalDesc.Height = portalHeight;
    portalDesc.MipLevels = 1;
    portalDesc.ArraySize = 1;
    portalDesc.Format = DXGI_FORMAT_D32_FLOAT;
    portalDesc.SampleDesc.Count = 1;
    portalDesc.SampleDesc.Quality = 0;
    portalDesc.Usage = D3D11_USAGE_DEFAULT;
    portalDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    portalDesc.CPUAccessFlags = 0;
    portalDesc.MiscFlags = 0;
    if (FAILED(dm->mainD3DDevice->CreateTexture2D(&portalDesc, NULL, &portalDepthStencil)))
    {
        gLastError = "Error creating portal depth stencil texture";
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC portalDescDSV = {};
    portalDescDSV.Format = portalDesc.Format;
    portalDescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    portalDescDSV.Texture2D.MipSlice = 0;
    portalDescDSV.Flags = 0;
    if (FAILED(dm->mainD3DDevice->CreateDepthStencilView(portalDepthStencil, &portalDescDSV, &portalDepthStencilView)))
    {
        gLastError = "Error creating portal depth stencil view";
        return false;
    }

    mirrorDesc = {};
    mirrorDesc.Width = portalWidth;
    mirrorDesc.Height = portalHeight;
    mirrorDesc.MipLevels = 1;
    mirrorDesc.ArraySize = 1;
    mirrorDesc.Format = DXGI_FORMAT_D32_FLOAT;
    mirrorDesc.SampleDesc.Count = 1;
    mirrorDesc.SampleDesc.Quality = 0;
    mirrorDesc.Usage = D3D11_USAGE_DEFAULT;
    mirrorDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
    mirrorDesc.CPUAccessFlags = 0;
    mirrorDesc.MiscFlags = 0;
    if (FAILED(dm->mainD3DDevice->CreateTexture2D(&mirrorDesc, NULL, &mirrorDepthStencil)))
    {
        gLastError = "Error creating portal depth stencil texture";
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC portalDescDSV2 = {};
    portalDescDSV2.Format = mirrorDesc.Format;
    portalDescDSV2.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    portalDescDSV2.Texture2D.MipSlice = 0;
    portalDescDSV2.Flags = 0;
    if (FAILED(dm->mainD3DDevice->CreateDepthStencilView(mirrorDepthStencil, &portalDescDSV2, &mirrorDepthStencilView)))
    {
        gLastError = "Error creating portal depth stencil view";
        return false;
    }

    return true;
}

bool Requirment4::InitScene()
{
    models[Floor].initModel();
    models[NormalMappingModel].initModel();
    models[ParalaxMappingModel].initModel();
    models[Shrek].initModel();
    models[TV].initModel();
    models[Mirror].initModel();

    lights[Pulsating].initModel();
    lights[Gradient].initModel();

    lights[Pulsating].getModel()->SetPosition(pulsatingLightStartPosition);
    lights[Pulsating].getModel()->SetScale(10);

    lights[Gradient].getModel()->SetPosition(gradientLightStartPosition);
    lights[Gradient].getModel()->SetScale(10);

    models[NormalMappingModel].getModel()->SetPosition(NormalMappingModelInitialPosition);
    models[ParalaxMappingModel].getModel()->SetPosition(ParalaxMappingModelInitialPosition);
    models[TV].getModel()->SetPosition(PortalInitialPosition);
    models[Mirror].getModel()->SetPosition(MirrorInitialPosition);
    models[Mirror].getModel()->SetRotation(MirrorInitialRoatation);
    models[Shrek].getModel()->SetPosition(ShrekInitialPosition);
    models[Shrek].getModel()->SetScale(ShrekInitialScale);

    camera->SetNearClip(0.01f);
    camera->SetFarClip(100000.0f);


    portalCamera = new Camera();
    portalCamera->SetPosition(PortalCameraInitialPosition);
    portalCamera->SetRotation(PortalCameraInitialRoatation);

    mirrorCamera = new Camera();
    mirrorCamera->SetPosition(MirrorInitialPosition);
    mirrorCamera->SetRotation({ ToRadians(20.0f), ToRadians(45.0f), 0 });
    return true;
}

void Requirment4::ReleaseResources()
{
    if (portalTexture) portalTexture->Release();
    if (portalRenderTarget) portalRenderTarget->Release();
    if (portalTextureSRV) portalTextureSRV->Release();
    if (portalDepthStencil) portalDepthStencil->Release();
    if (portalDepthStencilView) portalDepthStencilView->Release();
    if (mirrorTexture) mirrorTexture->Release();
    if (mirrorRenderTarget) mirrorRenderTarget->Release();
    if (mirrorTextureSRV) mirrorTextureSRV->Release();
    if (mirrorDepthStencil) mirrorDepthStencil->Release();
    if (mirrorDepthStencilView) mirrorDepthStencilView->Release();

    for (auto it = models.begin(); it != models.end(); ++it)
    {
        it->second.release();
    }
    for (auto it = lights.begin(); it != lights.end(); ++it)
    {
        it->second.release();
    }

    delete portalCamera;
    delete mirrorCamera;
}

void Requirment4::RenderCameraScene(Camera* camera)
{

    // Setting up mirror camera
    if (camera == mirrorCamera) {
        CVector3 mirrorNormal = { 0.0,0.0,-1.0 };

        CVector3 mirrorCenter = models[Mirror].getModel()->Position();
        std::vector<std::vector<float>> buffer = { {0.0},{0.0},{1.0} };
    
        float camRotX = this->camera->Rotation().x;
        float camRotY = this->camera->Rotation().y;
        float camRotZ = this->camera->Rotation().z;
        Matrix initialCamNormal(buffer);
        std::vector<std::vector<float>> buffer1 = {
            {1, 0, 0},
            {0, cos(camRotX), -sin(camRotX)},
            {0, sin(camRotX), cos(camRotX)}
        };
        Matrix xCamRot(buffer1);
        std::vector<std::vector<float>> buffer2 = {
            {cos(camRotY),0, sin(camRotY)},
            {0, 1, 0},
            {-sin(camRotY), 0, cos(camRotY)}
        };
        Matrix yCamRot(buffer2);
        std::vector<std::vector<float>> buffer3 = {
            {cos(camRotZ), -sin(camRotZ), 0},
            {sin(camRotZ), cos(camRotZ), 0},
            {0, 0, 1}
        };
        Matrix zCamRot(buffer3);
        Matrix cameraNormalMatrix = Matrix::matMul(yCamRot, Matrix::matMul(xCamRot, Matrix::matMul(zCamRot, initialCamNormal)));
        CVector3 cameraNormal = Normalise(CVector3(cameraNormalMatrix.getdata()[0][0], cameraNormalMatrix.getdata()[1][0], cameraNormalMatrix.getdata()[2][0]));
        CVector3 cameraPosition = this->camera->Position();
        float mirrorSurfaceDCoefficient = -(mirrorCenter.x * mirrorNormal.x + mirrorCenter.y * mirrorNormal.y + mirrorCenter.z * mirrorNormal.z);
        
        float denominator = (cameraNormal.x * mirrorNormal.x + cameraNormal.y * mirrorNormal.y + cameraNormal.z * mirrorNormal.z);
        float intersectionCoefficient = 0;
        if (denominator == 0)
        {
            // If denominator is 0, that means that camera vector is parallel with mirror surface
            intersectionCoefficient = 1e20;
        } 
        else
        {
            intersectionCoefficient = -(mirrorSurfaceDCoefficient + mirrorNormal.x * cameraPosition.x + mirrorNormal.y * cameraPosition.y + mirrorNormal.z * cameraPosition.z) /
                denominator;
        }
        CVector3 intersection = { cameraPosition.x + intersectionCoefficient * cameraNormal.x, cameraPosition.y + intersectionCoefficient * cameraNormal.y, cameraPosition.z + intersectionCoefficient * cameraNormal.z };
        mirrorCamera->SetPosition(intersection);
        mirrorCamera->SetRotation(models[Mirror].getModel()->Rotation());

    }

    perFrameGlobalConstants.viewMatrix = camera->ViewMatrix();;
    perFrameGlobalConstants.projectionMatrix = camera->ProjectionMatrix();
    perFrameGlobalConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(perFrameGlobalConstantBuffer, perFrameGlobalConstants, dm);
    dm->mainD3DContext->VSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer);  
    dm->mainD3DContext->PSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer);
    perFrameConstants.viewMatrix = camera->ViewMatrix();
    perFrameConstants.projectionMatrix = camera->ProjectionMatrix();
    perFrameConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(perFrameConstantBuffer, perFrameConstants, dm);
    dm->mainD3DContext->VSSetConstantBuffers(5, 1, &perFrameConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(5, 1, &perFrameConstantBuffer);

    // Floor rendering

    dm->mainD3DContext->VSSetShader(sm->vsMap["GeneralModelCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["CommonPixelShader_ps"], nullptr, 0);
    auto texture = models[Floor].getGPUTexture(0);
    dm->mainD3DContext->PSSetShaderResources(0, 1, &texture); 
    dm->mainD3DContext->PSSetSamplers(0, 1, &stateManager->anisotropic4xSampler);
    dm->mainD3DContext->OMSetBlendState(stateManager->noBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->OMSetDepthStencilState(stateManager->useDepthBufferState, 0);
    dm->mainD3DContext->RSSetState(stateManager->cullBackState);
    models[Floor].getModel()->Render(perModelConstants, perModelConstantBuffer);

    // Cube and teapot rendering with paralax and normal mapping
    
    dm->mainD3DContext->VSSetShader(sm->vsMap["ParalaxMapping_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["NormalMapping_ps"], nullptr, 0);
    dm->mainD3DContext->VSSetConstantBuffers(5, 1, &perFrameConstantBuffer);  
    dm->mainD3DContext->PSSetConstantBuffers(5, 1, &perFrameConstantBuffer);
    models[NormalMappingModel].setSampler(stateManager->anisotropic4xSampler);
    models[NormalMappingModel].render(perModelConstants, perModelConstantBuffer);
    
    dm->mainD3DContext->VSSetShader(sm->vsMap["ParalaxMapping_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["ParalaxMapping_ps"], nullptr, 0);
    models[ParalaxMappingModel].setSampler(stateManager->anisotropic4xSampler);
    dm->mainD3DContext->VSSetConstantBuffers(5, 1, &perFrameConstantBuffer); // First parameter must match constant buffer number in the shader 
    dm->mainD3DContext->PSSetConstantBuffers(5, 1, &perFrameConstantBuffer);
    models[ParalaxMappingModel].render(perModelConstants, perModelConstantBuffer);
    
    // Shrek rendering in cartoon style

    dm->mainD3DContext->VSSetShader(sm->vsMap["CartoonModelOutline_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["CartoonModelOutline_ps"], nullptr, 0);

    dm->mainD3DContext->OMSetBlendState(stateManager->noBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->OMSetDepthStencilState(stateManager->useDepthBufferState, 0);
    dm->mainD3DContext->RSSetState(stateManager->cullFrontState);
    models[Shrek].getModel()->Render(perModelConstants, perModelConstantBuffer);

    dm->mainD3DContext->VSSetShader(sm->vsMap["GeneralModelCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["CartoonModel_ps"], nullptr, 0);

    dm->mainD3DContext->RSSetState(stateManager->cullBackState);
    auto shrekColour = models[Shrek].getGPUTexture(0);
    dm->mainD3DContext->PSSetShaderResources(0, 1, &shrekColour);
    dm->mainD3DContext->PSSetSamplers(0, 1, &stateManager->anisotropic4xSampler);
    auto shrekCellMap = models[Shrek].getGPUTexture(1);
    dm->mainD3DContext->PSSetShaderResources(1, 1, &shrekCellMap);
    dm->mainD3DContext->PSSetSamplers(1, 1, &stateManager->pointSampler);
    models[Shrek].getModel()->Render(perModelConstants, perModelConstantBuffer);

    // TV rendering

    dm->mainD3DContext->PSSetShader(sm->psMap["Portal_ps"], nullptr, 0);
    dm->mainD3DContext->VSSetShader(sm->vsMap["GeneralModelCommon_vs"], nullptr, 0);
    texture = models[TV].getGPUTexture(0);
    dm->mainD3DContext->PSSetShaderResources(0, 1, &portalTextureSRV);
    dm->mainD3DContext->PSSetShaderResources(1, 1, &texture);
    models[TV].getModel()->Render(perModelConstants, perModelConstantBuffer);

    // Mirror rendering 
 
    // This if required so mirror camera doesn't render mirror, becuase the camera can be inside the surface
    if (camera != mirrorCamera) 
    {
        dm->mainD3DContext->PSSetShader(sm->psMap["Mirror_ps"], nullptr, 0);
        dm->mainD3DContext->PSSetShaderResources(0, 1, &mirrorTextureSRV);
        models[Mirror].getModel()->Render(perModelConstants, perModelConstantBuffer);
    }

    // Lights renderin
    dm->mainD3DContext->VSSetShader(sm->vsMap["LightModelsCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["LightModelsCommon_ps"], nullptr, 0);
    texture = lights[Pulsating].getGPUTexture(0);
    dm->mainD3DContext->PSSetShaderResources(0, 1, &texture);
    dm->mainD3DContext->PSSetSamplers(0, 1, &stateManager->anisotropic4xSampler);
    dm->mainD3DContext->OMSetBlendState(stateManager->additiveBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->OMSetDepthStencilState(stateManager->depthReadOnlyState, 0);
    dm->mainD3DContext->RSSetState(stateManager->cullNoneState);
    perModelConstants.objectColour = lights[Pulsating].getColour(); 
    lights[Pulsating].getModel()->Render(perModelConstants, perModelConstantBuffer);
    perModelConstants.objectColour = lights[Gradient].getColour();
    lights[Gradient].getModel()->Render(perModelConstants, perModelConstantBuffer);
}

void Requirment4::RenderScene()
{
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };  
  
    perFrameConstants.lightPulsatingColour = lights[Pulsating].getColour() * lights[Pulsating].getStrength();
    perFrameConstants.lightPulsatingPosition = lights[Pulsating].getModel()->Position();
    perFrameConstants.lightGradientColour = lights[Gradient].getColour() * lights[Gradient].getStrength();
    perFrameConstants.lightGradientPosition = lights[Gradient].getModel()->Position();
    perFrameConstants.ambientColour = ambientColour;
    perFrameConstants.specularPower = specularPower;
    perFrameConstants.cameraPosition = camera->Position();



    //// TV scene rendering ////

    dm->mainD3DContext->OMSetRenderTargets(1, &portalRenderTarget, portalDepthStencilView);
    dm->mainD3DContext->ClearRenderTargetView(portalRenderTarget, ClearColor);
    dm->mainD3DContext->ClearDepthStencilView(portalDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(portalWidth);
    vp.Height = static_cast<FLOAT>(portalHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    dm->mainD3DContext->RSSetViewports(1, &vp);
    RenderCameraScene(portalCamera);

    //// Mirror scene rendering ////

    dm->mainD3DContext->OMSetRenderTargets(1, &mirrorRenderTarget, mirrorDepthStencilView);
    dm->mainD3DContext->ClearRenderTargetView(mirrorRenderTarget, ClearColor);
    dm->mainD3DContext->ClearDepthStencilView(mirrorDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    D3D11_VIEWPORT vp2;
    vp2.Width = static_cast<FLOAT>(portalWidth);
    vp2.Height = static_cast<FLOAT>(portalHeight);
    vp2.MinDepth = 0.0f;
    vp2.MaxDepth = 1.0f;
    vp2.TopLeftX = 0;
    vp2.TopLeftY = 0;
    dm->mainD3DContext->RSSetViewports(1, &vp2);
    RenderCameraScene(mirrorCamera);

    //// Main scene rendering ////
    dm->mainD3DContext->OMSetRenderTargets(1, &dm->backBufferRenderTarget, dm->depthStencil);
    dm->mainD3DContext->ClearRenderTargetView(dm->backBufferRenderTarget, ClearColor);
    dm->mainD3DContext->ClearDepthStencilView(dm->depthStencil, D3D11_CLEAR_DEPTH, 1.0f, 0);

    vp.Width = static_cast<FLOAT>(gViewportWidth);
    vp.Height = static_cast<FLOAT>(gViewportHeight);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    dm->mainD3DContext->RSSetViewports(1, &vp);

    RenderCameraScene(camera);
    dm->swapChain->Present(0, 0);

}

void Requirment4::UpdateScene(float frameTime)
{
    lights[Pulsating].updateStrength(initialPulsatingStrength * (cos(currentChange) + 1) / 2);
    lights[Gradient].updateColour(initialGradientColor + (cos(currentChange) + 1) / 2 * (endGradientColor - initialGradientColor));
    models[Shrek].getModel()->Control(frameTime, Key_I, Key_K, Key_J, Key_L, Key_O, Key_U, Key_Period, Key_Comma);
    currentChange += lightChangingSpeed * frameTime;
}

void Requirment4::ReSetScene()
{
    lights[Pulsating].updateStrength(initialPulsatingStrength);
    lights[Gradient].updateColour(initialGradientColor);
    models[Shrek].getModel()->SetPosition(ShrekInitialPosition);

}
