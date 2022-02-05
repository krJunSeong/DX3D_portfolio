#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
    quad = new Quad();
    quad->tag = "Quad";
    quad->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");
    quad->Load();

    bunny = new Model("Bunny");
    bunny->Load();

    character = new ModelAnimator("character");
    character->ReadClip("Run", 1);
    character->PlayClip(0);
    character->Load();

    sphere = new Sphere();
    sphere->tag = "Sphere";
    sphere->GetMaterial()->Load("TextData/Materials/Stones.mat");
    sphere->Load();
}

LightScene::~LightScene()
{
    delete quad;
    delete bunny;
    delete character;
    delete sphere;
}

void LightScene::Update()
{
    quad->UpdateWorld();
    bunny->UpdateWorld();
    character->Update();
    sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
    quad->Render();
    bunny->Render();
    character->Render();
    sphere->Render();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
    //quad->GUIRender();
    //bunny->GUIRender();
    //character->GUIRender();
    //sphere->GUIRender();

    bunny->GetReader()->GUIRender();
    character->GetReader()->GUIRender();

    quad->GetMaterial()->GUIRender();
    sphere->GetMaterial()->GUIRender();
}
