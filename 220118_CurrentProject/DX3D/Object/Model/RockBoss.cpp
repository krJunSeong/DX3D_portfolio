#include "framework.h"

RockBoss::RockBoss()
{
	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "MonsterHpBar";

	CreateObject();
}

RockBoss::~RockBoss()
{
	delete instancing;
	for(RockPillar* rockPillar :  rockPillares)
		delete rockPillar;

	//delete rockShield;

	//for(CapsuleCollider* col : rockPillarCollideres)
	//	delete col;
}

void RockBoss::Update()
{
	instancing->Update();

	//rockShield->Update();
	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Update();

	//rockShield->Update();
}

void RockBoss::Render()
{
	instancing->Render();

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Render();

	//rockShield->Render();
}

void RockBoss::PostRender()
{
	for (RockPillar* rockPillar : rockPillares)
		rockPillar->PostRender();
}

void RockBoss::GUIRender()
{
	for (RockPillar* rockPillar : rockPillares)
		rockPillar->GUIRender();
	//rockShield->GUIRender();
}

/*
void RockPillar::CreateObject(int count)
{
	for(int i = 0; i < count; i++)
	{
		hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
		hpBar->tag = "RcokPillarBar";
		hpBar->Load();

		transform = Add();
		transform->position = {(float)Random(0, 50), 0.0f, (float)Random(0, 50)};

		CapsuleCollider* collider = new CapsuleCollider();
		collider->isActive = true;
		collider->tag = "PillarCollider";
		collider->scale = {1.5f,1.5f, 1.5f};
		collider->SetParent(transform);

		bodyCollideres.push_back(collider);
	}
}
*/

void RockBoss::CreateObject()
{
	// rock shiled 1°³ = º»Ã¼
	//rockShield = new ModelInstancing("RockShield");
	//rockShieldColliders = new CapsuleCollider();

	instancing = new ModelInstancing("RockPillar");

	for(int i = 0; i < 4; i++)
	{
		RockPillar* rockPillar = new RockPillar();
		Transform* trf = instancing->Add();
		trf->tag = "RockPillar";
		trf->scale *= 2.1f;
		trf->isActive = true;
		rockPillar->SetTransform(trf);

		rockPillar->GetCollider()->SetParent(trf);
		rockPillares.push_back(rockPillar);

		switch(i)
		{
			case 0:
				trf->position.x = rockShieldDistance;
				break;
			case 1:
				trf->position.x = -rockShieldDistance;
				break;
			case 2:
				trf->position.z = rockShieldDistance;
				break; 
			case 3:
				trf->position.z = -rockShieldDistance;
				break;
		}
	}
}