#include "framework.h"

RockShield::RockShield()
	: ModelInstancing("RockShield")
{
	CreateObject();
}

RockShield::~RockShield()
{
}

void RockShield::CreateObject()
{
	Transform* trf = Add();

	bodyCollider = new SphereCollider();
	bodyCollider->tag = "RockShieldCollider";
	bodyCollider->isActive = true;
	bodyCollider->scale = {1.5f, 1.5f, 1.5f};
	bodyCollider->SetParent(trf);
}

void RockShield::Update()
{
	ModelInstancing::Update();

	bodyCollider->UpdateWorld();
}

void RockShield::Render()
{
	ModelInstancing::Render();
	
	bodyCollider->Render();
}
