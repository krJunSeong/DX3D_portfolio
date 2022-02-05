#include "Framework.h"

Sky::Sky(wstring file)
{
    sphere = new Sphere();
    sphere->GetMaterial()->SetShader(L"Sky.hlsl");

    cubeMap = Texture::Add(file);

    rasterizerState[0] = new RasterizerState();
    rasterizerState[1] = new RasterizerState();
    rasterizerState[1]->FrontCounterClockwise(true);

    depthStencilState[0] = new DepthStencilState();
    depthStencilState[1] = new DepthStencilState();
    depthStencilState[1]->DepthEnable(false);
}

Sky::~Sky()
{
    delete sphere;

    delete rasterizerState[0];
    delete rasterizerState[1];

    delete depthStencilState[0];
    delete depthStencilState[1];
}

void Sky::Render()
{
    cubeMap->Set(10);

    rasterizerState[1]->SetState();
    depthStencilState[1]->SetState();

    sphere->Render();

    rasterizerState[0]->SetState();
    depthStencilState[0]->SetState();
}
