#include "Framework.h"
#include "ModelAnimatorScene.h"

ModelAnimatorScene::ModelAnimatorScene()
{
    string name = "Chuchu";

    modelAnimator = new ModelAnimator(name);
    modelAnimator->ReadClip("Idle", 0);
    modelAnimator->ReadClip("Run", 0);
    modelAnimator->ReadClip("Attack", 0);
    modelAnimator->ReadClip("HIT", 0);
    modelAnimator->ReadClip("DEATH", 0);

    
}

ModelAnimatorScene::~ModelAnimatorScene()
{
    delete modelAnimator;
}

void ModelAnimatorScene::Update()
{
    if (KEY_DOWN('1'))
        modelAnimator->PlayClip(0);
    if (KEY_DOWN('2'))
        modelAnimator->PlayClip(1);
    if (KEY_DOWN('3'))
        modelAnimator->PlayClip(2);
    if (KEY_DOWN('4'))
        modelAnimator->PlayClip(3);
    if (KEY_DOWN('5'))
        modelAnimator->PlayClip(4);

    modelAnimator->Update();
}

void ModelAnimatorScene::PreRender()
{
}

void ModelAnimatorScene::Render()
{
    modelAnimator->Render();
}

void ModelAnimatorScene::PostRender()
{
}

void ModelAnimatorScene::GUIRender()
{
    modelAnimator->GUIRender();
}
