#include "Framework.h"

Model::Model(string name)
{
    tag = name;

    reader = new ModelReader(name);
    reader->SetShader(L"Lighting.hlsl");

    worldBuffer = new WorldBuffer();
}

Model::~Model()
{
    delete reader;
    delete worldBuffer;
}

void Model::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);

    reader->Render();
}