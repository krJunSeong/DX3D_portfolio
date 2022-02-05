#include "Framework.h"

Zombie::Zombie()
    : ModelAnimator("Zombie"), state(IDLE), playingRootMotion(false)
{
    ReadClip("Idle");
    ReadClip("Run");
    ReadClip("Attack");
    ReadClip("Hit");
    ReadClip("Death");
    ReadClip("JumpAttack", 0, true);

    clips[ATTACK]->Events[0.2f] = bind(&Zombie::EnableSword, this);
    clips[ATTACK]->Events[0.5f] = bind(&Zombie::DisableSword, this);
    clips[ATTACK]->Events[0.7f] = bind(&Zombie::EndAttack, this);
    clips[HIT]->Events[0.8f] = bind(&Zombie::EndHit, this);
    clips[DEATH]->Events[0.8f] = bind(&Zombie::EndDeath, this);
    clips[JUMP_ATTACK]->Events[0.7f] = bind(&Zombie::EndJumpAttack, this);

    PlayClip(0);

    Load();

    sword = new Sword();
    sword->SetParent(&rightHand);
    sword->Load();

    collider = new SphereCollider();
    collider->tag = name + "Collider";
    collider->SetParent(this);
    collider->Load();

    hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");

    rootIndex = 1;

    light = Environment::Get()->AddLight();
    light->type = LightType::SPOT;
}

Zombie::~Zombie()
{
    delete sword;
    delete collider;
    delete hpBar;
}

void Zombie::Update()
{
    if (!isActive) return;

    light->position = position;
    light->direction = Forward() * -1.0f;

    if (MOUSE_CLICK(0))
    {
        Ray ray = CAM->ScreenPointToRay(mousePos);

        if (collider->RayCollision(ray))
        {
            Damage();
        }
    }

    if (KEY_DOWN(VK_SPACE))
        SetClip(ATTACK);

    if (KEY_DOWN('1'))
    {
        SetClip(JUMP_ATTACK);
    }

    if (KEY_DOWN(VK_ESCAPE))
        StopClip();

    SetRightHand();
    SetRoot();

    ModelAnimator::Update();

    sword->Update();
    collider->UpdateWorld();

    Vector3 barPos = position + Vector3(0, 17, 0);
    hpBar->position = CAM->WorldToScreenPoint(barPos);

    LerpHp();

    hpBar->Update();
}

void Zombie::Render()
{
    if (!isActive) return;

    ModelAnimator::Render();

    sword->Render();
    //collider->Render();
}

void Zombie::PostRender()
{
    if (!isActive) return;

    hpBar->Render();
}

void Zombie::GUIRender()
{
    Transform::GUIRender();

    sword->GUIRender();
    collider->GUIRender();
    sword->GetCollider()->GUIRender();

    sword->GetReader()->GUIRender();
}

void Zombie::SetRightHand()
{
    rightHand = GetTransformByNode(60) * world;
}

void Zombie::SetRoot()
{
    if (!playingRootMotion) return;
    if (!clips[frameBuffer->data.cur.clip]->IsRootMotion()) return;
    if (frameBuffer->data.next.clip >= 0) return;

    rootTransform = GetTransformByNode(rootIndex);
    NodeData rootNode = reader->GetNodes()[rootIndex];

    Vector3 nS, nR, nT;
    XMMatrixDecompose(&nS.data, &nR.data, &nT.data, rootNode.transform * world);

    Vector3 S, R, T;
    XMMatrixDecompose(&S.data, &R.data, &T.data, rootTransform * world);

    Vector3 deltaPos = T - nT;
    Vector3 deltaScale = S - nS;
    Vector3 deltaRot = RadianToQuaternion(R) - RadianToQuaternion(nR);

    position = startTransform.position + deltaPos;
    scale = startTransform.scale + deltaScale;
    rotation = startTransform.rotation + deltaRot;
}

void Zombie::SetClip(AnimState state)
{
    if (this->state != state)
    {
        if (clips[state]->IsRootMotion())
        {
            startTransform = *this;
            playingRootMotion = true;
        }

        this->state = state;
        PlayClip(state);
    }
}

void Zombie::EndAttack()
{
    SetClip(IDLE);
}

void Zombie::EndHit()
{
    SetClip(IDLE);
}

void Zombie::EndDeath()
{
    isActive = false;
}

void Zombie::EndJumpAttack()
{
    SetClip(IDLE);
    playingRootMotion = false;
    rotation = startTransform.rotation;
    //position = rootPosition;
}

void Zombie::EnableSword()
{
    sword->GetCollider()->isActive = true;
}

void Zombie::DisableSword()
{
    sword->GetCollider()->isActive = false;
}

void Zombie::LerpHp()
{
    lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

    hpBar->SetLerpValue(lerpHp);
}

void Zombie::Damage()
{
    hp -= 30.0f;

    hpBar->SetValue(hp);

    if (hp > 0)
        SetClip(HIT);
    else
        SetClip(DEATH);
}
