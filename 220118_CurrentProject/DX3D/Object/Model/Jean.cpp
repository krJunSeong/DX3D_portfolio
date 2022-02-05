#include "framework.h"

Jean::Jean()
	: ModelAnimator("Jean")
{
	Init();

	ReadClip("Idle");
	ReadClip("Run");
	ReadClip("Attack");
	ReadClip("Hit");
	ReadClip("Death");

	PlayClip(0);

	Transform::Load(); // transform->load()

	//CAM->SetTarget(this);

	clips[ATTACK]->Events[0.7f] = bind(&Jean::AttackEnd, this);
//	clips[JUMP]->Events[0.7f] = bind(&Jean::JumpEnd, this);
	
	bodyCollider = new CapsuleCollider(1, 4.0f);
	bodyCollider->tag = name + "Collider";
	bodyCollider->SetParent(this);
	bodyCollider->Transform::Load();

	mistsplitter = new Mistsplitter(&rightHand);
	wing = new Wing();
	wing->SetParent(this);

	hpBar = new Bar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_Bar_BG.png");

	skillUI = new Quad();
	skillUI->tag = "skillUI";
	skillUI->Load();
	skillUI->GetMaterial()->SetDiffuseMap(L"Textures/Star.png");
}

Jean::~Jean()
{
	delete bodyCollider;
	delete hpBar;
	delete skillUI;
	delete mistsplitter;
	delete wing;
}

void Jean::Update()
{
	SetRightHand();
	ModelAnimator::Update();
	bodyCollider->UpdateWorld();
	mistsplitter->Update();
	wing->Update();

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
		SetClip(ATTACK);

	else if (KEY_DOWN(VK_ESCAPE))
		StopClip();

	Vector3 barPos = position + Vector3(0, 17, 0);
	hpBar->position = CAM->WorldToScreenPoint(barPos);

	LerpHp();
	hpBar->Update();
}

void Jean::Render()
{
	ModelAnimator::Render();
	bodyCollider->Render();
	mistsplitter->Render();
	wing->Render();
}

void Jean::PostRender()
{
	hpBar->Render();
}

void Jean::GUIRender()
{
	Transform::GUIRender();
	bodyCollider->GUIRender();
	mistsplitter->GUIRender();
	mistsplitter->GetCollider()->GUIRender();
	wing->GUIRender();
	wing->GetReader()->GUIRender();
}

void Jean::SetClip(AnimState state)
{
	if (this->state != state)
	{
		this->state = state;
		PlayClip(state);
	}
}

void Jean::EndAttack()
{
	SetClip(IDLE);
}

void Jean::Init()
{
	state = IDLE;
	moveSpeed = 30.0f;
	gravity = 9.8f;
	jumpPower = 30.0f;
	rotSpeed = 5.0f;

	isAttack = false;
	isMove = false; 
	isJumping = false;
	isAirFloating = false;

	chuchus = InstancingMonsterManager::Get()->GetChuchus();
}

void Jean::Move()
{
	if (isAttack) return;
	//if (isJumping) return;

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
		rotation.y -= rotSpeed * DELTA;
	}
	else if (KEY_PRESS('D'))
	{
		isMove = true;
		rotation.y += rotSpeed * DELTA;
	}

	if (KEY_UP('W') || KEY_UP('A') || KEY_UP('S') || KEY_UP('D'))
	{
		isMove = false;
	}

	//if(isMove && isJumping) return;
	else if (isMove) SetClip(RUN);
	else SetClip(IDLE);
}


// ------------------- Attack ------------------------
void Jean::Attack()
{
	if (isJumping) return;

	InstancingMonsterManager::Get()->Collision(mistsplitter->GetCollider(), 100.0f);

	if (isAttack) return;


	if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse)  AttackInit(ATTACK);
}

void Jean::Damaged(float damage)
{
	hp -= 30.0f;
	hpBar->SetValue(hp);

	/* if (hp > 0)
		 SetClip();
	else
		SetClip(DEATH);*/
}

void Jean::AttackInit(AnimState state)
{
	isAttack = true;
	isMove = false;
	SetClip(state);
}

void Jean::AttackEnd()
{
	isAttack = false;
	SetClip(IDLE);
}


// ------------------- Move ----------------------
void Jean::Jump()
{
	if(position.y > 0 && KEY_DOWN(VK_SPACE))
	{
		isAirFloating = !isAirFloating;
		wing->isActive = !(wing->isActive);
	}

	if(isAirFloating)
	{
		//wing->isActive = true;
		moveSpeed = 3.0f;
		rotSpeed = 1.0f;
		float fallSpeed = 3.0f;
		position.y -= fallSpeed * DELTA;
	}

	if(!isAirFloating && isJumping) 
	{
		jumpPower -= gravity * DELTA;
		position.y += jumpPower * DELTA;
	}

	if (position.y < 0)
	{
		JumpEnd();
		return;
	}

	if (!isJumping && !isAirFloating && KEY_DOWN(VK_SPACE))
	{
		isJumping = true;
		//SetClip(JUMP);
	}
}
void Jean::JumpEnd()
{
	position.y = 0.0f;
	jumpPower = 30.0f;
	moveSpeed = 30.0f;
	rotSpeed = 5.0f;
	wing->isActive = false;

	isJumping = false;
	isAirFloating = false;
	SetClip(IDLE);
}

void Jean::LerpHp()
{
	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
}

void Jean::SkillAttack()
{
	ball->Fire();
}
