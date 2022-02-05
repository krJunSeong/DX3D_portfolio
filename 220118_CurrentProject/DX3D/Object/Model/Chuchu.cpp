#include "framework.h"
#include "Chuchu.h"

Chuchu::Chuchu()
    : ModelAnimator("Chuchu"), state(IDLE)
{
    ReadClip("Idle");
    ReadClip("Run");
    ReadClip("Attack");
    ReadClip("Hit");
    ReadClip("Death");

    Transform::Load();

    clips[ATTACK]->Events[0.7f] = bind(&Chuchu::EndAttack, this);

    PlayClip(0);

    collider = new CapsuleCollider(1, 4.5f);
    collider->tag = name + "BodyCollider";
    collider->SetParent(this);
    collider->Load();

    attackCollider = new BoxCollider(Vector3(3.0f, 3.0f, 3.0f));
    attackCollider->tag = "ChuchuAttackCollider";
    attackCollider->SetParent(&rightHand);
    attackCollider->Load();

    hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
    hpBar->tag = "ChuchuHpBar";
    //hpBar->scale *= 5.0f;
}

Chuchu::~Chuchu()
{
    delete collider;
    delete attackCollider;
    delete hpBar;
}

void Chuchu::Update()
{
    if (!isActive) return;

    SetRightHand();

    ModelAnimator::Update();
    collider->UpdateWorld();
    attackCollider->UpdateWorld();

    Vector3 barPos = position + Vector3(0, 5.5f, 0);
    hpBar->position = CAM->WorldToScreenPoint(barPos);

    float distance = Distance(CAM->position, position);
    hpBar->scale.x = hpBarScaleRate / distance;
    hpBar->scale.y = hpBarScaleRate / distance;

    if(MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse)
        SetClip(ATTACK);

    LerpHp();

    hpBar->Update();
}

void Chuchu::Render()
{
    if(!isActive) return;

    ModelAnimator::Render();
    collider->Render();
    attackCollider->Render();
}

void Chuchu::PostRender()
{
    hpBar->Render();
}

void Chuchu::GUIRender()
{
    if(!isActive) return;

    Transform::GUIRender();
    collider->GUIRender();
    attackCollider->GUIRender();
}

void Chuchu::Damage(float damage)
{
    hp -= damage;
}

void Chuchu::Init()
{
    hp = 100.0f;
    lerpHp = 100.0f;

    isActive = true;
    collider->isActive = true;
    attackCollider->isActive = false;

    state = IDLE;
}

void Chuchu::SetRightHand()
{
    rightHand = GetTransformByNode(110) * world;
}

void Chuchu::SetClip(AnimState state)
{
    if (this->state != state)
    {
        this->state = state;
        PlayClip(state);
    }
}

void Chuchu::EndAttack()
{
    SetClip(IDLE);
}

void Chuchu::LerpHp()
{
    lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

    hpBar->SetLerpValue(lerpHp);
}