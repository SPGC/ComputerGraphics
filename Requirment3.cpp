#include "Requirment3.h"
#include "../State.h"

using namespace R3;

bool Requirment3::InitGeometry()
{
    models.insert(std::make_pair(Cube, Object("Cube", dm)));
    lights.insert(std::make_pair(MainLight, Light("MainLight", dm, lightColour, lightStrength)));

    if (!(
        models[Cube].initMesh("cube.x")
        && lights[MainLight].initMesh("light.x")
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
        models[Cube].initTextures("StoneDiffuseSpecular.dds", 0)
        && models[Cube].initTextures("WoodDiffuseSpecular.dds", 1)
        && lights[MainLight].initTextures("Flare.jpg", 0)
        ))
    {
        return false;
    }
    return true;
}

bool Requirment3::InitScene()
{
    models[Cube].initModel();
    lights[MainLight].initModel();

    lights[MainLight].getModel()->SetPosition(lightPosition);
    lights[MainLight].getModel()->SetScale(10);

    models[Cube].getModel()->SetScale(0.5);
    models[Cube].getModel()->SetPosition(cubePosition);
    return true;
}

void Requirment3::ReleaseResources()
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

void Requirment3::RenderScene()
{


    perFrameGlobalConstants.viewMatrix = camera->ViewMatrix();;
    perFrameGlobalConstants.projectionMatrix = camera->ProjectionMatrix();
    perFrameGlobalConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(perFrameGlobalConstantBuffer, perFrameGlobalConstants, dm);

    dm->mainD3DContext->VSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer);

    perFrameConstants.viewMatrixR3 = camera->ViewMatrix();
    perFrameConstants.projectionMatrixR3 = camera->ProjectionMatrix();
    perFrameConstants.viewProjectionMatrixR3 = camera->ViewProjectionMatrix();
    perFrameConstants.lightColourR3 = lights[MainLight].getColour() * lights[MainLight].getStrength();
    perFrameConstants.lightPositionR3 = lights[MainLight].getModel()->Position();
    perFrameConstants.ambientColourR3 = ambientColour;
    perFrameConstants.specularPowerR3 = specularPower;
    perFrameConstants.cameraPositionR3 = camera->Position();
    perFrameConstants.fadingStateR3 = fadingState;
    UpdateConstantBuffer(perFrameConstantBuffer, perFrameConstants, dm);

    // Cube rendering

    dm->mainD3DContext->VSSetConstantBuffers(4, 1, &perFrameConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(4, 1, &perFrameConstantBuffer);

    dm->mainD3DContext->OMSetBlendState(stateManager->noBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->RSSetState(stateManager->cullBackState);

    dm->mainD3DContext->VSSetShader(sm->vsMap["GeneralModelCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["Requirment3_ps"], nullptr, 0);
    models[Cube].setSampler(stateManager->anisotropic4xSampler);
    models[Cube].render(perModelConstants, perModelConstantBuffer);

    // Lights rendering

    dm->mainD3DContext->VSSetShader(sm->vsMap["LightModelsCommon_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["LightModelsCommon_ps"], nullptr, 0);

    auto texture = lights[MainLight].getGPUTexture(0);

    dm->mainD3DContext->PSSetShaderResources(0, 1, &texture);
    dm->mainD3DContext->PSSetSamplers(0, 1, &stateManager->anisotropic4xSampler);
    dm->mainD3DContext->OMSetBlendState(stateManager->additiveBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->OMSetDepthStencilState(stateManager->depthReadOnlyState, 0);
    dm->mainD3DContext->RSSetState(stateManager->cullNoneState);
    perModelConstants.objectColour = lights[MainLight].getColour(); 
    lights[MainLight].getModel()->Render(perModelConstants, perModelConstantBuffer);

}

void Requirment3::UpdateScene(float frameTime)
{
    models[Cube].getModel()->Control(frameTime, Key_I, Key_K, Key_J, Key_L, Key_O, Key_U, Key_Period, Key_Comma);
    fadingState += fadingSpeed * frameTime;
}

void Requirment3::ReSetScene()
{
    models[Cube].getModel()->SetPosition(cubePosition);
    fadingState = 0;
}
