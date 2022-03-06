#include "Framework.h"
#include "ParticleToolScene.h"

ParticleToolScene::ParticleToolScene()
{
    particle = new ParticleSystem();
}

ParticleToolScene::~ParticleToolScene()
{
    delete particle;
}

void ParticleToolScene::Update()
{
    if (KEY_DOWN('1'))
    {
        particle->Save("TextData/Particles/HorizonStar.fx");
    }

    if (KEY_DOWN('2'))
    {
        particle->Load("TextData/Particles/HorizonStar.fx");
    }

    particle->Update();
}

void ParticleToolScene::PreRender()
{
}

void ParticleToolScene::Render()
{
    particle->Render();
}

void ParticleToolScene::PostRender()
{
}

void ParticleToolScene::GUIRender()
{
    particle->GUIRender();
}
