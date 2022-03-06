#include "framework.h"

RockBoss::RockBoss()
	: state(IDLE)
{
	CreateObject();
}

RockBoss::~RockBoss()
{
	delete instancing;
	for (RockPillar* rockPillar : rockPillares)
		delete rockPillar;

	delete rockShieldInstancing;
	delete rockShield;

	//for(CapsuleCollider* col : rockPillarCollideres)
	//	delete col;
}

void RockBoss::Update()
{
	if(!rockShield->isActive) return;

	if(isPhase1) Phase1();
	else Phase2();

	instancing->Update();			// rockshiled
	rockShieldInstancing->Update();

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Update();

	rockShield->Update();
}

void RockBoss::Render()
{
	if (!rockShield->isActive) return;

	instancing->Render();
	for (RockPillar* rockPillar : rockPillares)
		rockPillar->Render();

	rockShieldInstancing->Render();
	rockShield->Render();
}

void RockBoss::PostRender()
{
	if (!rockShield->isActive) return;

	for (RockPillar* rockPillar : rockPillares)
		rockPillar->PostRender();

	rockShield->PostRender();
}

void RockBoss::GUIRender()
{
	if (!rockShield->isActive) return;

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

		if(rockShield->GetHp() < Phase2Hp)
		{
			isPhase1 = false;
			isPhase2 = true;
		}
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
	// Move rockShield --> player,
	if (isCrash)
	{
		if (rockShield->position.y < land->GetHeight(rockShield->position) + 1.0f)
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
		}
	}
	else
		rockShield->position.y += MoveSpeed *  DELTA;

	// Prepare Attacking, Limit rockShield position.y
	if (rockShield->position.y >= 30.0f)
	{
		//if(player == nullptr) 
		Vector3 temp = player->position - rockShield->position;
		originPlayerPosDir = temp.GetNormalized();
		isCrash = true;
	}

}

void RockBoss::Floating()
{
	floatingTime += DELTA;
	idleTime += DELTA;
	//rockShield->GetCollider()->isActive = true;

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
	rockShield->rotation.y += SpinSpeed * DELTA;
	int count = RockPillar::GetPillarCount();

	// count > 1 : 기둥이 세워져 있어요
	// count < 1 : 기둥이 없어요.
	if(count == 0)	// 기둥이 하나도 없다면
	{
		if(rockShield->position.y > land->GetHeight(rockShield->position)) 
			rockShield->position.y -= 3.0f * DELTA;

		initTime += DELTA;
		if(initTime >= LimitInitTime)
		{
			isPillaresInit = true;// 0
			initTime = 0.0f;
			attackTime = 0.0f;
		}
	}
	else 
	{	// 2페이즈 공격
		// 기둥이 세워져 있다면
		attackTime += DELTA;

		if(attackTime >= LimitAttackTime)
		{
			player->Damaged(count * Att);
			attackTime = 0.0f;

			ParticleManager::Get()->Play("HorizonStar", rockShield->position + Vector3(0, 20.0f, 0));

			for (RockPillar* pillar : rockPillares)
			{
				if(!pillar->isActive) continue;
				ParticleManager::Get()->Play("UpStar", pillar->position);
			}

			CAM->Shake(2.0f, 2.0f);
		}
	}

	if (count > 0)
	{
		rockShield->GetCollider()->isActive = false;
		rockShield->GetHpBar()->isActive = false;
		//Floating();
	}
	else 
	{
		rockShield->GetCollider()->isActive = true;
		rockShield->GetHpBar()->isActive = true;
	}

	if(isPillaresInit)
	{
		int i = 0;
		for(RockPillar* pillar : rockPillares)
		{
			pillar->Init(land);

			pillar->position.x = Random(land->position.x + 3.0f, land->GetSize().x);
			pillar->position.z = Random(land->position.y + 3.0f, land->GetSize().y);

			ParticleManager::Get()->Play("UpStar", pillar->position);
		}

		isIniting = true;
		CAM->Shake(2.0f, 2.0f);
		isPillaresInit = false;
	}
		
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
		insTrf->isActive = false;
		rockPillar->isActive = false;
		rockPillar->SetTransform(insTrf);
		rockPillares.push_back(rockPillar);
	}

	rockShieldInstancing = new ModelInstancing("RockShield");
	rockShield = new RockShield();
	Transform* temp = rockShieldInstancing->Add();
	temp->tag = "RockShield";
	temp->isActive = true;
	rockShield->SetTransform(temp);
}

void RockBoss::InitRockPillares()
{

}
