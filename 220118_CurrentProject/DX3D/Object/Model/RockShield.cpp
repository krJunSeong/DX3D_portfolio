#include "framework.h"

RockShield::RockShield()
{
	CreateObject();
}

RockShield::~RockShield()
{
	delete bodyCollider;
	delete hpBar;
}

void RockShield::CreateObject()
{
	Transform::tag = "RockShield";
	Transform::isActive = true;
	Transform::Load();

	floatingSpeed = 2.0f;

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "RockShieldHpBar";
	hpBar->Load();

	bodyCollider = new SphereCollider();
	bodyCollider->tag = "RockShieldCollider";
	bodyCollider->isActive = true;
	bodyCollider->scale = {1.5f, 1.5f, 1.5f};
	bodyCollider->SetParent(this);
	bodyCollider->Load();
}

void RockShield::Update()
{
	Transform::UpdateWorld();
	bodyCollider->UpdateWorld();
	HpControll();

	//Floating();
}

void RockShield::Render()
{
	if (!Transform::isActive) return;

	bodyCollider->Render();
}

void RockShield::GUIRender()
{
	Transform::GUIRender();
	bodyCollider->GUIRender();
}

void RockShield::PostRender()
{
	if (!Transform::isActive) return;

	hpBar->Render();
}

void RockShield::HpControll()
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

void RockShield::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}

void RockShield::Damaged(float damage)
{
	if(!bodyCollider->isActive) return;

	hp -= damage;

	hpBar->SetValue(hp);

	if (hp <= 0.0f)
	{
		Transform::isActive = false;
	}
}

void RockShield::Floating()
{
	floatingTime += DELTA;
		
	if(floatingTime > FloatingDuration)
	{
		floatingTime = 0.0f;
		floatingSpeed = -floatingSpeed;
	}

	position.y += floatingSpeed * DELTA;
}
