#include "Framework.h"
#include "ModelInstancingScene.h"

ModelInstancingScene::ModelInstancingScene()
{
    modelInstancing = new ModelInstancing("Lee");

    for (float z = -10.0f; z < 10.0f; z++)
    {
        for (float x = -10.0f; x < 10.0f; x++)
        {
            Transform* transform = modelInstancing->Add();

            transform->position = { x, 0, z };
            transform->scale *= 0.5f;
            //Model* model = new Model("Lee");
            //model->position = { x, 0, z };
            //model->UpdateWorld();
            //models.push_back(model);
        }
    }
}

ModelInstancingScene::~ModelInstancingScene()
{
    //for (Model* model : models)
        //delete model;
    delete modelInstancing;
}

void ModelInstancingScene::Update()
{
    modelInstancing->Update();
}

void ModelInstancingScene::PreRender()
{
}

void ModelInstancingScene::Render()
{
    //for (Model* model : models)
        //model->Render();
    modelInstancing->Render();
}

void ModelInstancingScene::PostRender()
{
}

void ModelInstancingScene::GUIRender()
{
    modelInstancing->GUIRender();
}
