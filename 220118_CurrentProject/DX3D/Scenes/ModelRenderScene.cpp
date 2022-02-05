#include "Framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
    string name = "RockShield"; 

    model = new Model(name);
    model->GetReader()->SetShader(L"Lighting.hlsl");

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Additive();
}

ModelRenderScene::~ModelRenderScene()
{
    delete model;

    delete blendState[0];
    delete blendState[1];
}

void ModelRenderScene::Update()
{
    //model->rotation.y = CAM->rotation.y;

    model->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
    //blendState[1]->SetState();
    model->Render();
    //blendState[0]->SetState();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
    model->GetReader()->GUIRender();
    model->GUIRender();
}
