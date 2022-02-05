#include "framework.h"

Amber::Amber()
	: ModelAnimator("Amber"), state(IDLE)
{
    ReadClip("Idle");
    ReadClip("Run");
    ReadClip("Attack");

    Transform::Load();

    clips[ATTACK]->Events[0.7f] = bind(&Amber::EndAttack, this);

    PlayClip(0);

    collider = new CapsuleCollider(1, 4.5f);
    collider->tag = name + "Collider";
    collider->SetParent(this);
    collider->Load();

    hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
    hpBar->position = {CENTER_X, 100, 0};
    hpBar->scale *= 5.0f;
}

Amber::~Amber()
{
    delete collider;
    delete hpBar;
}

void Amber::Update()
{
    SetRightHand();

    ModelAnimator::Update();

    collider->UpdateWorld();

    LerpHp();

    hpBar->Update();
}

void Amber::Render()
{
    ModelAnimator::Render();
    collider->Render();
}

void Amber::PostRender()
{
    hpBar->Render();
}

void Amber::GUIRender()
{
    collider->GUIRender();
}

void Amber::Damage()
{
}

void Amber::SetRightHand()
{
    rightHand = GetTransformByNode(60) * world;
}

void Amber::SetClip(AnimState state)
{
    if (this->state != state)
    {
        this->state = state;
        PlayClip(state);
    }
}

void Amber::EndAttack()
{
    SetClip(IDLE);
}

void Amber::LerpHp()
{
    lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

    hpBar->SetLerpValue(lerpHp);
}
