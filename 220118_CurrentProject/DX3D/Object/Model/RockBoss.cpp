#include "framework.h"

RockBoss::RockBoss()
	: state(IDLE)
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

	delete hpBar;

	delete rockShieldInstancing;
	delete rockShield;

	//for(CapsuleCollider* col : rockPillarCollideres)
	//	delete col;
}

void RockBoss::Update()
{
	Phase1();
	
	instancing->Update();
	rockShieldInstancing->Update();

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Update();

	rockShield->Update();
}

void RockBoss::Render()
{
	instancing->Render();
	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Render();

	rockShieldInstancing->Render();
	rockShield->Render();
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
	
	rockShield->GUIRender();
}

void RockBoss::Phase1()
{
	if (state == IDLE)
	{
		Floating();
	}
}

void RockBoss::Floating()
{
	floatingTime += DELTA;

	if (floatingTime > FloatingDuration)
	{
		floatingTime = 0.0f;
		floatingSpeed = -floatingSpeed;
	}

	rockShield->position.y += floatingSpeed * DELTA;
}

void RockBoss::CreateObject()
{
	instancing = new ModelInstancing("RockPillar");

	for(int i = 0; i < 4; i++)
	{
		RockPillar* rockPillar = new RockPillar();
		Transform* insTrf = instancing->Add();
		insTrf->tag = "RockPillar";
		insTrf->scale *= 2.1f;
		insTrf->isActive = true;
		rockPillar->SetTransform(insTrf);
		rockPillares.push_back(rockPillar);

		switch(i)
		{
			case 0:
				rockPillar->position.x = rockShieldDistance;
				break;
			case 1:
				rockPillar->position.x = -rockShieldDistance;
				break;
			case 2:
				rockPillar->position.z = rockShieldDistance;
				break; 
			case 3:
				rockPillar->position.z = -rockShieldDistance;
				break;
		}
	}

	rockShieldInstancing = new ModelInstancing("RockShield");
	rockShield = new RockShield();
	Transform* temp = rockShieldInstancing->Add();
	temp->tag = "RockShield";
	temp->isActive = true;
	rockShield->SetTransform(temp);
}