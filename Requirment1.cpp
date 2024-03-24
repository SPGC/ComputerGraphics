#include "Requirment1.h"
#include "../State.h"
using namespace R1;

bool Requirment1::InitGeometry()
{
    models.insert(std::make_pair(Floor, Object("Floor", dm)));
    models.insert(std::make_pair(Teapot, Object("Teapot", dm)));
    lights.insert(std::make_pair(Pulsating, Light("Pulsating", dm, pulsatingColour, initialPulsatingStrength)));
    lights.insert(std::make_pair(Gradient, Light("Gradient", dm, initialGradientColor, initialPulsatingStrength)));

    if (!(
        models[Floor].initMesh("floor.x")
        && models[Teapot].initMesh("teapot.x")
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
        && models[Teapot].initTextures("StoneDiffuseSpecular.dds", 0)
        && lights[Pulsating].initTextures("Flare.jpg", 0)
        && lights[Gradient].initTextures("Flare.jpg", 0)
        ))
    {
        return false;
    }
    return true;
}

bool Requirment1::InitScene()
{
    models[Floor].initModel();
    models[Teapot].initModel();
    lights[Pulsating].initModel();
    lights[Gradient].initModel();

    lights[Pulsating].getModel()->SetPosition(pulsatingLightStartPosition);
    lights[Pulsating].getModel()->SetScale(10);

    lights[Gradient].getModel()->SetPosition(gradientLightStartPosition);
    lights[Gradient].getModel()->SetScale(10);

    models[Teapot].getModel()->SetScale(1);
    models[Teapot].getModel()->SetPosition(teapotInitialPosition);
    return true;
}

void Requirment1::ReleaseResources()
{
    for (auto it = models.begin(); it != models.end(); ++it)
    {
        it->second.release();
    }
    for (auto it = lights.begin(); it != lights.end(); ++it)
    {
        it->second.release();
    }
}

void Requirment1::RenderScene()
{
    perFrameGlobalConstants.viewMatrix = camera->ViewMatrix();;
    perFrameGlobalConstants.projectionMatrix = camera->ProjectionMatrix();
    perFrameGlobalConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(perFrameGlobalConstantBuffer, perFrameGlobalConstants, dm);

    dm->mainD3DContext->VSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer);

    perFrameConstants.viewMatrixR1 = camera->ViewMatrix();
    perFrameConstants.projectionMatrixR1 = camera->ProjectionMatrix();
    perFrameConstants.viewProjectionMatrixR1 = camera->ViewProjectionMatrix();
    perFrameConstants.lightPulsatingColourR1 = lights[Pulsating].getColour() * lights[Pulsating].getStrength();
    perFrameConstants.lightPulsatingPositionR1 = lights[Pulsating].getModel()->Position();
    perFrameConstants.lightGradientColourR1 = lights[Gradient].getColour() * lights[Gradient].getStrength();
    perFrameConstants.lightGradientPositionR1 = lights[Gradient].getModel()->Position();
    perFrameConstants.ambientColourR1 = ambientColour;
    perFrameConstants.specularPowerR1 = specularPower;
    perFrameConstants.cameraPositionR1 = camera->Position();
    UpdateConstantBuffer(perFrameConstantBuffer, perFrameConstants, dm);

    dm->mainD3DContext->VSSetConstantBuffers(2, 1, &perFrameConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(2, 1, &perFrameConstantBuffer);

    // Floor rendering

    dm->mainD3DContext->VSSetShader(sm->vsMap["GeneralModelCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["Requirment1_ps"], nullptr, 0);

    auto texture = models[Floor].getGPUTexture(0);
    dm->mainD3DContext->PSSetShaderResources(0, 1, &texture); 
    dm->mainD3DContext->PSSetSamplers(0, 1, &stateManager->anisotropic4xSampler);

    dm->mainD3DContext->OMSetBlendState(stateManager->noBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->OMSetDepthStencilState(stateManager->useDepthBufferState, 0);
    dm->mainD3DContext->RSSetState(stateManager->cullBackState);

    models[Floor].getModel()->Render(perModelConstants, perModelConstantBuffer);

    // Teapot rendering

    models[Teapot].setSampler(stateManager->anisotropic4xSampler);
    models[Teapot].render(perModelConstants, perModelConstantBuffer);

    // Lights rendering

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

void Requirment1::UpdateScene(float frameTime)
{
    lights[Pulsating].updateStrength(initialPulsatingStrength * (cos(currentChange) + 1) / 2);
    lights[Gradient].updateColour(initialGradientColor + (cos(currentChange) + 1) / 2 * (endGradientColor - initialGradientColor));
    models[Teapot].getModel()->Control(frameTime, Key_I, Key_K, Key_J, Key_L, Key_O, Key_U, Key_Period, Key_Comma);
    currentChange += lightChangingSpeed * frameTime;
}

void Requirment1::ReSetScene()
{
    lights[Pulsating].updateStrength(initialPulsatingStrength);
    lights[Gradient].updateColour(initialGradientColor);
    models[Teapot].getModel()->SetPosition(teapotInitialPosition);
}
