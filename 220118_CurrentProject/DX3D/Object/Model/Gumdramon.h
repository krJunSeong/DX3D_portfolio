#pragma once

class Gumdramon : public ModelAnimator
{
public:
	enum AnimState 
	{ IDLE, RUN, JUMP, MELEEATTACK, SKILLATTACK, HIT, DEATH} state;

	Gumdramon();
	~Gumdramon();
	
	Collider* collider;

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void SetClip(AnimState state);

	void EndAttack();

private:
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float moveSpeed;

	Bar* hpBar;
	class SkillBall* ball;
	Quad* skillUI;

	bool isAttack;
	bool isMove;
	bool isJumping;

	void Move();
	void Attack();
	void Damaged(float damage);

	void SkillAttack();

	void AttackInit(AnimState state);
	void AttackEnd();

	void Jump();
	void JumpEnd();

	void LerpHp();
};