#include "Framework.h"
#include "ModelAnimatorInstancingScene.h"

ModelAnimatorInstancingScene::ModelAnimatorInstancingScene()
{
    modelAnimatorInstancing = new ModelAnimatorInstancing("zombie");
    modelAnimatorInstancing->ReadClip("Idle");
    modelAnimatorInstancing->ReadClip("Run");
    modelAnimatorInstancing->ReadClip("Attack");

    for (float z = -5.0f; z < 5.0f; z++)
    {
        for (float x = -5.0f; x < 5.0f; x++)
        {
            Transform* transform = modelAnimatorInstancing->Add();

            transform->position = { x, 0, z };
            transform->scale *= 0.01f;            
        }
    }
}

ModelAnimatorInstancingScene::~ModelAnimatorInstancingScene()
{
    delete modelAnimatorInstancing;
}

void ModelAnimatorInstancingScene::Update()
{
    modelAnimatorInstancing->Update();
}

void ModelAnimatorInstancingScene::PreRender()
{
}

void ModelAnimatorInstancingScene::Render()
{
    modelAnimatorInstancing->Render();
}

void ModelAnimatorInstancingScene::PostRender()
{
}

void ModelAnimatorInstancingScene::GUIRender()
{
    ImGui::SliderInt("InstanceID", &instanceID, 0, MAX_INSTANCE);
    ImGui::SliderInt("SelectClip", &selectClip, 0, 2);
    if (ImGui::Button("PlayClip"))
        modelAnimatorInstancing->PlayClip(instanceID, selectClip);
}
