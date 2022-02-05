#include "framework.h"

RockPillar::RockPillar()
	:ModelInstancing("RockPillar")
{
	bodyCollider = new CapsuleCollider(1, 1);
	bodyCollider->tag = "RockPillarCollider";
	bodyCollider->Load();

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "RockPillarHpBar";
	hpBar->Load();
}

RockPillar::~RockPillar()
{
	delete bodyCollider;
	delete hpBar;

	// �̰�
	//for(CapsuleCollider* col : bodyCollideres)
	//	delete col;
}

void RockPillar::SetParent(Transform* trf)
{
	for(Transform* transform : transforms)
		transform->SetParent(trf);
}

void RockPillar::Update()
{
	if(!transform->isActive) return;

	ModelInstancing::Update(); 

	bodyCollider->UpdateWorld();
	HpControll();

	// �̰�
	//for(CapsuleCollider* col : bodyCollideres)
	//	col->UpdateWorld();
}

void RockPillar::Render()
{
	if (!transform->isActive) return;
	ModelInstancing::Render();

	bodyCollider->Render();
	// �̰�
	//for (CapsuleCollider* col : bodyCollideres)
	//	col->Render();
}

void RockPillar::GUIRender()
{
	transform->GUIRender();
	bodyCollider->GUIRender();
	// �̰�
	//for (CapsuleCollider* col : bodyCollideres)
	//	col->GUIRender();
}

void RockPillar::PostRender()
{
	if (!transform->isActive) return;

	hpBar->Render();
}

void RockPillar::Init()
{
	transform->isActive = true;
	bodyCollider->isActive = true;

	hp = 100.0f;
	hpBar->SetValue(hp);
}

void RockPillar::HpControll()
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

void RockPillar::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}
