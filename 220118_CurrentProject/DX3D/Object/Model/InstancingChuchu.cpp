#include "framework.h"

int InstancingChuchu::deathCount = 0;
InstancingChuchu::InstancingChuchu(UINT instanceID)
	:instanceID(instanceID)
	,actionState(ActionState::IDLE)
	,animState(AnimState::IDLE)
	,isAttack(false)
	,isMove(false)
	,isJumping(false) //moveSpeed(30.0f),
{
	bodyCollider = new CapsuleCollider(1, 3);
	bodyCollider->tag = "ChuchuBodyCollider";
	bodyCollider->Load();

	attackCollider = new BoxCollider();
	attackCollider->tag = "ChuchuAttackCollider";
	//attackCollider->SetParent(transform);
	
	attackCollider->Load();

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "ChuchuHpBar";
	hpBar->Load();
}

InstancingChuchu::~InstancingChuchu()
{
	delete bodyCollider;
	delete attackCollider;
	delete hpBar;
}

void InstancingChuchu::Update()
{
	if (!transform->isActive) return;

	SetRightHand();

	bodyCollider->UpdateWorld();
	attackCollider->UpdateWorld();

	HpControl();

	if(animState == DEATH) return;
	if(animState == HIT) return;

	transform->position.y = terrain->GetHeight(transform->position);

	ActionIdle();
	ActionTrace();
}

void InstancingChuchu::Render()
{
	if (!transform->isActive) return;

	bodyCollider->Render();
	attackCollider->Render();
}

void InstancingChuchu::GUIRender()
{
	transform->GUIRender();
	bodyCollider->GUIRender();
	attackCollider->GUIRender();

	hpBar->GUIRender();
}

void InstancingChuchu::PostRender()
{
	if (!transform->isActive) return;

	hpBar->Render();
}

void InstancingChuchu::AttackEnd()
{
	isAttack = false;
	SetClip(IDLE);
}

void InstancingChuchu::HitEnd()
{
	bodyCollider->isActive = true;
	SetClip(IDLE);
}

void InstancingChuchu::DeathEnd()
{
	transform->isActive = false;
	deathCount++;
}

void InstancingChuchu::SetClip(AnimState state, float speed)
{
	if (this->animState != state)
	{
		this->animState = state;
		instancing->PlayClip(instanceID, state);
	}
}

void InstancingChuchu::SpawnInit()
{
	transform->isActive = true;
	bodyCollider->isActive = true;
	attackCollider-> isActive = true;
	attackCollider->SetParent(&rightHand);

	isAttack = false;
	isMove = false;
	isJumping = false;

	actionState = ActionState::IDLE;

	hp = 100.0f;
	hpBar->SetValue(hp);

	SetClip(IDLE);
}

void InstancingChuchu::Damaged(float damage)
{
	hp -= damage;
	hpBar->SetValue(hp);

	bodyCollider->isActive = false;

	if (hp <= 0.0f)
	{
		SetClip(DEATH);
	}
	else
		SetClip(HIT);
}

void InstancingChuchu::HpControl()
{
	Vector3 barPos = transform->position + Vector3(0, 5.5f, 0);
	hpBar->position = CAM->WorldToScreenPoint(barPos);

	float distance = Distance(CAM->position, transform->position);
	hpBar->scale.x = hpBarScaleRate / distance;
	hpBar->scale.y = hpBarScaleRate / distance;

	hpBar->SetValue(hp);

	LerpHp();
	hpBar->Update();
}

void InstancingChuchu::ActionIdle()
{
	if (actionState != ActionState::IDLE) return;

	SetClip(IDLE);

	Vector3 vec3Distance = player->position - transform->position;
	float distance = vec3Distance.Length();

	if(distance <= 30.0f) actionState = ActionState::TRACE;
}

void InstancingChuchu::ActionTrace()
{
	if(actionState != ActionState::TRACE) return;

	Vector3 vec3Distance = player->position - transform->position;
	float distance = vec3Distance.Length();

	// 일정 거리 안이면 어택한다.
	if(distance <= 5.0f) 
	{
		SetClip(ATTACK);
		return;
	}

	// 아니라면 로테이션 돌리면서 이동한다.
	Vector3 dir = player->position - transform->position;
	transform->position += dir.GetNormalized() * movespeed * DELTA;

	Vector3 rot = Vector3::Cross(transform->Forward(), dir.GetNormalized());;
	if (rot.y < 0)
		transform->rotation.y += rotSpeed * DELTA;
	else if (rot.y > 0)
		transform->rotation.y -= rotSpeed * DELTA;

	SetClip(RUN);
}

void InstancingChuchu::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}

void InstancingChuchu::SetEvents()
{
	instancing->AddEvent(instanceID, HIT, 0.8f, bind(&InstancingChuchu::HitEnd, this));
	instancing->AddEvent(instanceID, DEATH, 0.8f, bind(&InstancingChuchu::DeathEnd, this));
}
