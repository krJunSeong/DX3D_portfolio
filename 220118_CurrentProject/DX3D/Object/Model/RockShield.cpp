#include "framework.h"

RockShield::RockShield()
{
	CreateObject();
}

RockShield::~RockShield()
{
	delete bodyCollider;
}

void RockShield::CreateObject()
{
	Transform::tag = "RockShield";
	Transform::isActive = true;
	Transform::Load();

	floatingSpeed = 2.0f;

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
	//Floating();
}

void RockShield::Render()
{
	bodyCollider->Render();
}

void RockShield::GUIRender()
{
	Transform::GUIRender();
	bodyCollider->GUIRender();
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
