#include "framework.h"

Gumdramon::Gumdramon()
	: ModelAnimator("Gumdramon"), state(IDLE),
	isAttack(false), isMove(false), moveSpeed(30.0f),
	isJumping(false)
{
	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Jump");
	ReadClip("MeleeAttack");
	ReadClip("SkillAttack");
	ReadClip("Hit");
	ReadClip("Death");

	PlayClip(0);

	Transform::Load(); // transform->load()

	//CAM->SetTarget(this);

	clips[MELEEATTACK]->Events[0.7f] = bind(&Gumdramon::AttackEnd, this);
	clips[SKILLATTACK]->Events[0.7f] = bind(&Gumdramon::AttackEnd, this);
	clips[SKILLATTACK]->Events[0.4f] = bind(&Gumdramon::SkillAttack, this);
	clips[JUMP]->Events[0.7f] = bind(&Gumdramon::JumpEnd, this);

	collider = new SphereCollider();
	collider->SetParent(this);
	collider->tag = name + "Collider";
	collider->Load();

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");

	ball = new SkillBall();
	ball->SetTarget(this);

	skillUI = new Quad();
	skillUI->tag = "skillUI";
	skillUI->Load();
	skillUI->GetMaterial()->SetDiffuseMap(L"Textures/Star.png");
}

Gumdramon::~Gumdramon()
{
	delete collider;
	delete hpBar;
	delete ball;
	delete skillUI;
}

void Gumdramon::Update()
{
	Jump();
	Move();
	Attack();

	if (KEY_DOWN('0'))
		SetClip(IDLE);

	else if (KEY_DOWN('1'))
		SetClip(RUN);

	else if (KEY_DOWN('2'))
		SetClip(JUMP);

	else if (KEY_DOWN('3'))
		SetClip(MELEEATTACK);

	else if (KEY_DOWN('4'))
		SetClip(SKILLATTACK);

	else if (KEY_DOWN(VK_ESCAPE))
		StopClip();
	
	Vector3 barPos = position + Vector3(0, 17, 0);
	hpBar->position = CAM->WorldToScreenPoint(barPos);

	ModelAnimator::Update();
	collider->UpdateWorld();

	ball->Update();

	LerpHp();
	hpBar->Update();
	skillUI->UpdateWorld();
}

void Gumdramon::Render()
{
	if (!isActive) return;

	ModelAnimator::Render();
	collider->Render();
	ball->Render();
}

void Gumdramon::PostRender()
{
	if (!isActive) return;

	hpBar->Render();
	//skillUI->Render();
}

void Gumdramon::GUIRender()
{
	skillUI->GUIRender();
	skillUI->GetMaterial()->GUIRender();
}

void Gumdramon::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}

void Gumdramon::EndAttack()
{
	SetClip(IDLE);
}

void Gumdramon::Move()
{
	if(isAttack) return;
	if(isJumping) return;

	if (KEY_PRESS('W'))		  
	{
		isMove = true;
		position -= Forward() * moveSpeed * DELTA;
	} 
	else if (KEY_PRESS('S'))
	{
		isMove = true;
		position += Forward() * moveSpeed * DELTA;
	}

	if (KEY_PRESS('A')) 
	{
		isMove = true;
		rotation.y -= 5 * DELTA;
	}

	else if (KEY_PRESS('D'))
	{
		isMove = true;
		rotation.y += 5 * DELTA;
	}

	if(KEY_UP('W') || KEY_UP('A') || KEY_UP('S') || KEY_UP('D'))
	{
		isMove = false;
	}

	//if(isMove && isJumping) return;
	else if(isMove) SetClip(RUN);
	else SetClip(IDLE);
}


// ------------------- Attack ------------------------
void Gumdramon::Attack()
{
	if(isAttack) return;
	if(isJumping) return;

	if (MOUSE_CLICK(0))  AttackInit(MELEEATTACK);
	else if (MOUSE_CLICK(1))  
	{
		AttackInit(SKILLATTACK);
	}
}

void Gumdramon::Damaged(float damage)
{
	hp -= 30.0f;

	hpBar->SetValue(hp);

	/* if (hp > 0)
		 SetClip();
	else
		SetClip(DEATH);*/
}

void Gumdramon::AttackInit(AnimState state)
{
	isAttack = true;
	isMove = false;
	SetClip(state);
}

void Gumdramon::AttackEnd()
{
	isAttack = false;
	SetClip(IDLE);
}

void Gumdramon::JumpEnd()
{
	isJumping = false;
	SetClip(IDLE);
}

// ------------------- Move ----------------------
void Gumdramon::Jump()	
{
 	if(KEY_DOWN(VK_SPACE))
	{
		isJumping = true;
		SetClip(JUMP);
	}
}

void Gumdramon::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}

void Gumdramon::SkillAttack()
{
	ball->Fire();
}
