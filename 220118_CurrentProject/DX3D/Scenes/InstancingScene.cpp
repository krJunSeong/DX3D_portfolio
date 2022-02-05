#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
    sphere = new Sphere(0.5f);
    sphere->GetMaterial()->SetShader(L"Instancing.hlsl");
    
    for (float y = -20.0f; y < 20.0f; y++)
    {
        for (float x = -20.0f; x < 20.f; x++)
        {
            InstanceData data;
            data.transform = XMMatrixTranslation(x, y, 0.0f);
            data.transform = XMMatrixTranspose(data.transform);

            instanceDatas.push_back(data);
        }
    }

    instanceBuffer = new VertexBuffer(instanceDatas.data(), sizeof(InstanceData), instanceDatas.size());
}

InstancingScene::~InstancingScene()
{
    delete sphere;
    delete instanceBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
    instanceBuffer->IASet(1);

    sphere->SetRender();

    DC->DrawIndexedInstanced(sphere->GetIndexCount(), instanceDatas.size(), 0, 0, 0);
}

void InstancingScene::PostRender()
{
}

void InstancingScene::GUIRender()
{
}
