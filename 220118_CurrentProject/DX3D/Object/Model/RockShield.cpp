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

	bodyCollider = new SphereCollider();
	bodyCollider->tag = "RockShieldCollider";
	bodyCollider->isActive = true;
	bodyCollider->scale = {1.5f, 1.5f, 1.5f};
	bodyCollider->SetParent(this);
	bodyCollider->Load();

	hpBar = new Bar(L"Textures/UI/ProgressBar_Fill_Health_Yellow.png", L"Textures/UI/ProgressBar2_Background.png");
	hpBar->tag = "RockShieldHpBar";
	hpBar->Load();

	header = new Quad();
	header->tag = "RockShieldHeader";
	header->GetMaterial()->SetDiffuseMap(L"Textures/UI/Window_Header.png");
	header->GetMaterial()->SetShader(L"Texture.hlsl");
	header->Load();

	for(int i = 0; i < 4; i++)
	{
		shiledCount[i] = new Quad();
		shiledCount[i]->tag = "ShiledCount" + to_string(i);
		shiledCount[i]->GetMaterial()->SetDiffuseMap(L"Textures/UI/LevelFrame_Star_0.png");
		shiledCount[i]->GetMaterial()->SetShader(L"Texture.hlsl");
		shiledCount[i]->Load();
	}

}

void RockShield::Update()
{
	Transform::UpdateWorld();
	bodyCollider->UpdateWorld();
	HpControll();
	header->UpdateWorld();

	for (int i = 0; i < 4; i++)
	{
		shiledCount[i]->UpdateWorld();
	}

	if(isHit)
	{
		hitTime += DELTA;
		if(hitTime >= LimitHitTime)
		{
			bodyCollider->isActive = true;
			isHit = false;
			hitTime = 0.0f;
		}
	}
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
	hpBar->GUIRender();
	header->GUIRender();

	for (int i = 0; i < 4; i++)
	{
		shiledCount[i]->rotation.z += 3.0f * DELTA;
		shiledCount[i]->GUIRender();
	}
}

void RockShield::PostRender()
{
	if (!Transform::isActive) return;

	hpBar->Render();
	header->Render();
	Font::Get()->RenderText("RockShield", "default", Float2(CENTER_X, 30));

	for (int i = 0; i < RockPillar::GetPillarCount(); i++)
	{
		shiledCount[i]->Render();
	}
}

void RockShield::HpControll()
{
	Vector3 barPos = Transform::position + Vector3(0, 5.5f, 0);
	//hpBar->position = Vector3(600.0f, 600.0f, 0);
	//hpBar->position = CAM->WorldToScreenPoint(barPos);

	//float distance = Distance(CAM->position, Transform::position);
	//hpBar->scale.x = 8.0f;
	//hpBar->scale.y = 2.0f;

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
	if(isHit) return;
	hp -= damage;

	bodyCollider->isActive = false;
	isHit = true;
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
