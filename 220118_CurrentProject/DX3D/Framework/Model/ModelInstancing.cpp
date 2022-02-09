#include "Framework.h"

ModelInstancing::ModelInstancing(string name)
{
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);

    reader = new ModelReader(name);
    reader->SetShader(L"Instancing.hlsl");

    size = reader->GetSize();
}

ModelInstancing::~ModelInstancing()
{
    for (Transform* transform : transforms)
        delete transform;

    delete instanceBuffer;
    delete reader;
}

void ModelInstancing::Update()
{
    drawCount = 0;

    for (UINT i = 0; i < transforms.size(); i++)
    {
        if (!transforms[i]->isActive) continue;

        transforms[i]->UpdateWorld();

        if (!FRUSTUM->ContainBox(transforms[i]->GlobalPos(), size * transforms[i]->GlobalScale())) continue;
        
        instanceDatas[drawCount].transform = XMMatrixTranspose(transforms[i]->GetWorld());
        drawCount++;
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}

void ModelInstancing::Render()
{
    instanceBuffer->IASet(1);

    reader->Render(drawCount);
}

void ModelInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);

    for (Transform* transform : transforms)
        transform->GUIRender();
}

Transform* ModelInstancing::Add()
{
    Transform* transform = new Transform();
    transform->tag = reader->GetName() + "_" + to_string(transforms.size());
    transforms.push_back(transform);

    return transform;
}
