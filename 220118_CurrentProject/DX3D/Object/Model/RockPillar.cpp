#include "framework.h"

RockPillar::RockPillar(int count)
	:ModelInstancing("RockPillar")
{
	CreateObject(count);
}

RockPillar::~RockPillar()
{
	for(CapsuleCollider* col : bodyCollideres)
		delete col;
}

void RockPillar::CreateObject(int count)
{
	for(int i = 0; i < count; i++)
	{
		Transform* trf = Add();
		trf->position = {(float)Random(0, 50), 0.0f, (float)Random(0, 50)};

		CapsuleCollider* collider = new CapsuleCollider();
		collider->isActive = true;
		collider->tag = "PillarCollider";
		collider->scale = {1.5f,1.5f, 1.5f};
		collider->SetParent(trf);

		bodyCollideres.push_back(collider);
	}
}

void RockPillar::SetParent(Transform* trf)
{
	for(Transform* transform : transforms)
		transform->SetParent(trf);
}

void RockPillar::Update()
{
	ModelInstancing::Update(); 

	for(CapsuleCollider* col : bodyCollideres)
		col->UpdateWorld();
}

void RockPillar::Render()
{
	ModelInstancing::Render();

	for (CapsuleCollider* col : bodyCollideres)
		col->Render();
}
