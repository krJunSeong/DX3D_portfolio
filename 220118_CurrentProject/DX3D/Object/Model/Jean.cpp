#include "framework.h"

Jean::Jean()
	: ModelAnimator("Jean")
{
	Init();

	ReadClip("Idle");		//	Idle	Animation Load
	ReadClip("Run");		//	Run		Animation Load
	ReadClip("Attack");		//	Attack	Animation Load
	ReadClip("Hit");		//	Hit		Animation Load
	ReadClip("Death");		//	Death	Animation Load

	PlayClip(0);			// ù ���۽� �⺻ �ִϸ��̼� ����

	Transform::Load();		// ���õ� Ʈ������ �ε�

	clips[ATTACK]->Events[0.5f] = bind(&Jean::AttackEnd, this);	// ���� ����� ó�� �Լ�
	clips[HIT]->Events[0.7f] = bind(&Jean::HitEnd, this);		// �ǰ� ����� ó�� �Լ�
	clips[DEATH]->Events[0.7f] = bind(&Jean::EndDeath, this);	// ���� ����� ó�� �Լ�

//	clips[JUMP]->Events[0.7f] = bind(&Jean::JumpEnd, this);
	
	bodyCollider = new CapsuleCollider(1, 4.0f);
	bodyCollider->tag = name + "Collider";
	bodyCollider->SetParent(this);
	bodyCollider->Transform::Load();

	mistsplitter = new Mistsplitter(&rightHand);
	mistsplitter->GetCollider()->isActive = false;

	wing = new Wing();
	wing->SetParent(this);

	skillUI = new Quad();
	skillUI->tag = "skillUI";
	skillUI->Load();
	skillUI->GetMaterial()->SetDiffuseMap(L"Textures/Star.png");

	playerUI = new PlayerUI();
}

Jean::~Jean()
{
	delete bodyCollider;
	delete skillUI;
	delete mistsplitter;
	delete wing;
	delete playerUI;
}

void Jean::Update()
{
	if(isDeath) return;
	SetRightHand();
	ModelAnimator::Update();
	bodyCollider->UpdateWorld();
	mistsplitter->Update();
	wing->Update();

	Jump();
	Move();
	Attack();


	if (!bodyCollider->isActive)
	{
		hitTime += DELTA;
		if (hitTime >= LimitHitTime)
		{
			bodyCollider->isActive = true;
			isHit = false;
			hitTime = 0.0f;
		}
	}


	Vector3 barPos = position + Vector3(0, 17, 0);

	LerpHp();

	playerUI->Update();
}

void Jean::Render()
{
	if(isDeath) return;
	ModelAnimator::Render();
	bodyCollider->Render();
	mistsplitter->Render();
	wing->Render();
}

void Jean::PostRender()
{
	playerUI->Render();
}

void Jean::GUIRender()
{
	Transform::GUIRender();
	bodyCollider->GUIRender();
	mistsplitter->GUIRender();
	mistsplitter->GetCollider()->GUIRender();
	wing->GUIRender();
	wing->GetReader()->GUIRender();
	playerUI->GUIRender();
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

void Jean::EndDeath()
{
	isDeath = true;
}

void Jean::Init()
{
	state = IDLE;
	moveSpeed = 100.0f;
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
	if (isHit) return;
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

	position.y = land->GetHeight(position);

	//if(isMove && isJumping) return;
	if (isMove) SetClip(RUN);
	else SetClip(IDLE);
}


// ------------------- Attack ------------------------
void Jean::Attack()
{
	// ���� ���̸� ����
	if (isJumping) return;

	// if(���콺 Ŭ���ϸ�) ���ø�� && isAttack = true; ���� �ݶ��̴� on
	if (MOUSE_CLICK(0) && !ImGui::GetIO().WantCaptureMouse)  AttackInit(ATTACK);

	if (!isAttack) return;

	InstancingMonsterManager::Get()->Collision(mistsplitter->GetCollider(), att);
	boss->Collision(mistsplitter->GetCollider(), att); // �����Ǵ� �κ� 
}

void Jean::Damaged(float damage)
{
	if (!bodyCollider->isActive) return;
	if (isHit) return;

	hp -= damage;

	bodyCollider->isActive = false;
	isHit = true;
	playerUI->SetHp(hp);

	 if (hp > 0)
	 {
		 SetClip(HIT);
		 isHit = true;
	 }
	else
	 {
		SetClip(DEATH);
		 //Transform::isActive = false;
	 }
}

void Jean::HitEnd()
{
	isHit = false;
	isAttack = false;
	SetClip(IDLE);
}

void Jean::AttackInit(AnimState state)
{
	isAttack = true;
	isMove = false;
	mistsplitter->GetCollider()->isActive = true;
	SetClip(state);
}

void Jean::AttackEnd()
{
	isAttack = false;
	mistsplitter->GetCollider()->isActive = false;
	SetClip(IDLE);
	// �߰�: ���� ������ ���� bodyCollider �� ��
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
}

void Jean::SkillAttack()
{
	ball->Fire();
}
