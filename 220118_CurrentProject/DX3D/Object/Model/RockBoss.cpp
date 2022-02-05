#include "framework.h"

RockBoss::RockBoss()
{
	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");
	hpBar->tag = "MonsterHpBar";

	CreateObject();
}

RockBoss::~RockBoss()
{
	//delete rockShield;

	delete rockPillar;
	
	//for(CapsuleCollider* col : rockPillarCollideres)
	//	delete col;
}

void RockBoss::Update()
{
	//rockShield->Update();
	rockPillar->Update();
}

void RockBoss::Render()
{
	//rockShield->Render();
	rockPillar->Render();
}

void RockBoss::GUIRender()
{
}

void RockBoss::CreateObject()
{
	// rock shiled 1개 = 본체
	//rockShield = new ModelInstancing("RockShield");
	//rockShieldColliders = new CapsuleCollider();

	rockPillar = new RockPillar(4);

	// rock pillar 4개 = 기둥
	/*for(int i = 0; i < 4; i++)
	{
		Transform* trf = rockPillar->Add();

		trf->position.x = Random(0, 50);
		trf->position.z = Random(0, 50);

		//CapsuleCollider* rockPillarCollider = new CapsuleCollider(1, 3);
		//rockPillarCollideres.push_back(rockPillarCollider);
	}*/
}