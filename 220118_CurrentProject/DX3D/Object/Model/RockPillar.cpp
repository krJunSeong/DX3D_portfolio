#include "framework.h"

int RockPillar::PillarCount = 4;

RockPillar::RockPillar()
{
	Transform::tag = "RockPillar";
	Transform::isActive = true;

	bodyCollider = new CapsuleCollider(1, 2);
	bodyCollider->tag = "RockPillarCollider";
	bodyCollider->SetParent(this);
	bodyCollider->Load();

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "RockPillarHpBar";
	hpBar->Load();
}

RockPillar::~RockPillar()
{
	delete bodyCollider;
	delete hpBar;

	// 이관
	//for(CapsuleCollider* col : bodyCollideres)
	//	delete col;
}

void RockPillar::Update()
{
	if(!Transform::isActive) return;

	if (isIniting)
	{
		// 기둥의 포지션이 땅보다 낮다면 계속 올려라.
		if (position.y < 0) position.y += DELTA;
		else isIniting = false;
	}

	UpdateWorld();
	bodyCollider->UpdateWorld();
	HpControll();

	// 이관
	//for(CapsuleCollider* col : bodyCollideres)
	//	col->UpdateWorld();
}

void RockPillar::Render()
{
	if (!Transform::isActive) return;
	
	bodyCollider->Render();
	// 이관
	//for (CapsuleCollider* col : bodyCollideres)
	//	col->Render();
}

void RockPillar::GUIRender()
{
	Transform::GUIRender();
	bodyCollider->GUIRender();
	// 이관
	//for (CapsuleCollider* col : bodyCollideres)
	//	col->GUIRender();
}

void RockPillar::PostRender()
{
	if (!Transform::isActive) return;

	hpBar->Render();
}

void RockPillar::Damaged(float damage)
{
	if (!bodyCollider->isActive) return;

	hp -= damage;

	hpBar->SetValue(hp);

	if(hp <= 0.0f)
	{
		Transform::isActive = false;
		objTransform->isActive = false;
		bodyCollider->isActive = false;

		position.y -= 3.0f;

		PillarCount--;
	}
}

void RockPillar::Init()
{
	//Spawn, Camera 흔들림 효과 추가?

	Transform::isActive = true;
	objTransform->isActive = true;
	bodyCollider->isActive = true;
	isIniting = true;

	hp = 100.0f;
	hpBar->SetValue(hp);

	PillarCount++;
}

void RockPillar::HpControll()
{
	Vector3 barPos = Transform::position + Vector3(0, 5.5f, 0);
	hpBar->position = CAM->WorldToScreenPoint(barPos);

	float distance = Distance(CAM->position, Transform::position);
	hpBar->scale.x = hpBarScaleRate / distance;
	hpBar->scale.y = hpBarScaleRate / distance;

	hpBar->SetValue(hp);

	LerpHp();
	hpBar->Update();
}

void RockPillar::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}
