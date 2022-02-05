#include "framework.h"

SkillBall::SkillBall(Vector3 pos)
	: limitTime(0), target(nullptr)
{
	isActive = false;

	ball = new Sphere();
	ball->GetMaterial()->SetDiffuseMap(L"Textures/Star.png");
	ball->position = pos;
	ball->SetParent(&GetWorld());

	collider = new SphereCollider();
	collider->tag = "SKillBallCollider";
	collider->SetParent(ball);
	collider->position = pos;
}

SkillBall::~SkillBall()
{
	delete ball;
	delete collider;
}

void SkillBall::Update()
{
	if(!isActive) return;

	limitTime += DELTA;
	if(limitTime > 1.0f)
	{
		isActive = false;
		limitTime = 0.0f;
		return;
	}

	position -= tempFs * 10 * DELTA;

	//ball->position = position;
	//ball->rotation = rotation;

	UpdateWorld();
	ball->UpdateWorld();
	collider->UpdateWorld();
}

void SkillBall::Render()
{
	if (!isActive) return;

	ball->Render();
	collider->Render();
}

void SkillBall::Fire()
{
	isActive = true;
	tempFs = target->Forward();						// 쏠 때 방향

	position = target->GlobalPos() - tempFs * 5.0f; // 시작위치 지정
	position.y = target->GlobalPos().y + 3.0f;		// offset

	rotation = target->rotation;
}
