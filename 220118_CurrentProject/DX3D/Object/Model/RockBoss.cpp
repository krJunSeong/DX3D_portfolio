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
	for (RockPillar* rockPillar : rockPillares)
		delete rockPillar;

	delete hpBar;

	delete rockShieldInstancing;
	delete rockShield;

	//for(CapsuleCollider* col : rockPillarCollideres)
	//	delete col;
}

void RockBoss::Update()
{
	if(hp > Phase2Hp) Phase1();
	else Phase2();

	instancing->Update();			// rockshiled
	rockShieldInstancing->Update();

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Update();

	rockShield->Update();
}

void RockBoss::Render()
{
	instancing->Render();
	rockShieldInstancing->Render();

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Render();

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
	{
		rockPillar->GUIRender();
	}

	rockShield->GUIRender();
	rockShield->GetCollider()->GUIRender();
}


void RockBoss::Collision(Collider* collider, float damage)
{
	for(RockPillar* rockPillar : rockPillares)
	{
		if(rockPillar->GetCollider()->Collision(collider))
			rockPillar->Damaged(damage);
	}

	if(rockShield->GetCollider()->Collision(collider))
	{
		rockShield->Damaged(damage);
	}
}

void RockBoss::Phase1()
{
	if (state == IDLE)
	{
		Floating();
	}

	if (state == ATTACK)
	{
		Phase1Attack();
	}
}

void RockBoss::Phase1Attack()
{
	// Move rockShield --> player
	if (isCrash)
	{
		if (rockShield->position.y < quad->position.y + 1.0f)
		{
			isCrash = false;
			state = IDLE;
			return;
		}

		// move to origin player pos
		rockShield->position += originPlayerPosDir * MoveSpeed * DELTA;
		rockShield->rotation.y += SpinSpeed * DELTA;

		if(rockShield->GetCollider()->Collision(player->GetCollider()))
		{
			player->Damaged(Att);
			rockShield->GetCollider()->isActive = false;
		}
	}

	// Prepare Attacking, Limit rockShield position.y
	if (rockShield->position.y >= 10.0f)
	{
		//if(player == nullptr) 
		Vector3 temp = player->position - rockShield->position;
		originPlayerPosDir = temp.GetNormalized();
		isCrash = true;
	}
	else
		rockShield->position.y += DELTA;
}

void RockBoss::Floating()
{
	floatingTime += DELTA;
	idleTime += DELTA;
	rockShield->GetCollider()->isActive = true;

	if (idleTime >= LimitIdleTime && (rockShield->position.y <= 2.0f))
	{
		state = ATTACK;
		idleTime = 0.0f;
		return;
	}

	if (floatingTime > FloatingDuration)
	{
		floatingTime = 0.0f;
		floatingSpeed = -floatingSpeed;
	}

	rockShield->position.y += floatingSpeed * DELTA;
}

void RockBoss::Phase2()
{
	// 이거 데미지 처리시? 딴 곳에서 ㅊ ㅓ리하는 게 좋아보임. 너무 여러번 하니까.
	if(RockPillar::GetPillarCount() < 4)
		rockShield->GetCollider()->isActive = false;
	else { rockShield->GetCollider()->isActive = true; }
		
}

void RockBoss::CreateObject()
{
	instancing = new ModelInstancing("RockPillar");

	for (int i = 0; i < 4; i++)
	{
		RockPillar* rockPillar = new RockPillar();
		Transform* insTrf = instancing->Add();
		insTrf->tag = "RockPillar";
		insTrf->scale *= 2.1f;
		insTrf->isActive = true;
		rockPillar->isActive = true;
		rockPillar->SetTransform(insTrf);
		rockPillares.push_back(rockPillar);

		switch (i)
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