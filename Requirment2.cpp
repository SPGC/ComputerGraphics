#include "Requirment2.h"
#include "../State.h"

using namespace R2;

bool Requirment2::InitGeometry()
{
    models.insert(std::make_pair(Sphere, Object("Sphere", dm)));
    lights.insert(std::make_pair(MainLight, Light("MainLight", dm, lightColour, lightStrength)));

    if (!(
        models[Sphere].initMesh("sphere.x")
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
        models[Sphere].initTextures("tiles1.jpg", 0)
        && lights[MainLight].initTextures("Flare.jpg", 0)
        ))
    {
        return false;
    }
    return true;
}

bool Requirment2::InitScene()
{
    models[Sphere].initModel();
    lights[MainLight].initModel();

    lights[MainLight].getModel()->SetPosition(lightPosition);
    lights[MainLight].getModel()->SetScale(10);

    models[Sphere].getModel()->SetScale(0.5);
    models[Sphere].getModel()->SetPosition(spherePosition);
    return true;
}

void Requirment2::ReleaseResources()
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

void Requirment2::RenderScene()
{
    perFrameGlobalConstants.viewMatrix = camera->ViewMatrix();;
    perFrameGlobalConstants.projectionMatrix = camera->ProjectionMatrix();
    perFrameGlobalConstants.viewProjectionMatrix = camera->ViewProjectionMatrix();
    UpdateConstantBuffer(perFrameGlobalConstantBuffer, perFrameGlobalConstants, dm);

    dm->mainD3DContext->VSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(0, 1, &perFrameGlobalConstantBuffer);

    perFrameConstants.viewMatrixR2 = camera->ViewMatrix();
    perFrameConstants.projectionMatrixR2 = camera->ProjectionMatrix();
    perFrameConstants.viewProjectionMatrixR2 = camera->ViewProjectionMatrix();
    perFrameConstants.lightColourR2 = lights[MainLight].getColour() * lights[MainLight].getStrength();
    perFrameConstants.lightPositionR2 = lights[MainLight].getModel()->Position();
    perFrameConstants.ambientColourR2 = ambientColour;
    perFrameConstants.specularPowerR2 = specularPower;
    perFrameConstants.cameraPositionR2 = camera->Position();
    perFrameConstants.wiggleStateR2 = wiggleState;
    perFrameConstants.wiggleScaleR2 = scalingFactor;
    perFrameConstants.sphereCenterR2 = models[Sphere].getModel()->Position();
    UpdateConstantBuffer(perFrameConstantBuffer, perFrameConstants, dm);

    // Sphere rendering

    dm->mainD3DContext->VSSetConstantBuffers(3, 1, &perFrameConstantBuffer); 
    dm->mainD3DContext->PSSetConstantBuffers(3, 1, &perFrameConstantBuffer);

    dm->mainD3DContext->OMSetBlendState(stateManager->noBlendingState, nullptr, 0xffffff);
    dm->mainD3DContext->RSSetState(stateManager->cullBackState);

    dm->mainD3DContext->VSSetShader(sm->vsMap["Requirment2_vs"], nullptr, 0);
    dm->mainD3DContext->PSSetShader(sm->psMap["Requirment2_ps"], nullptr, 0);
    models[Sphere].setSampler(stateManager->anisotropic4xSampler);
    models[Sphere].render(perModelConstants, perModelConstantBuffer);

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

void Requirment2::UpdateScene(float frameTime)
{
    models[Sphere].getModel()->Control(frameTime, Key_I, Key_K, Key_J, Key_L, Key_O, Key_U, Key_Period, Key_Comma);
    wiggleState += wiggleSpeed * frameTime;
}

void Requirment2::ReSetScene()
{
    models[Sphere].getModel()->SetPosition(spherePosition);
    wiggleState = 0;
}
