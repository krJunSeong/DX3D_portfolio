#pragma once

class Jean : public ModelAnimator
{
private:
	enum AnimState
	{
		IDLE, RUN, ATTACK, HIT, DEATH, JUMP
	} state;

	Collider* bodyCollider;
	Matrix rightHand;

	Mistsplitter* mistsplitter;
	Model* zephyrus;
	Wing* wing;
	vector<InstancingChuchu*> chuchus;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float moveSpeed;
	float rotSpeed;
	float jumpPower;
	float gravity;

	Bar* hpBar;
	class SkillBall* ball;
	Quad* skillUI;

	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isAirFloating;

private:
	void Init();
	void Move();
	void Attack();
	void Damaged(float damage);

	void SkillAttack();

	void AttackInit(AnimState state);
	void AttackEnd();

	void Jump();
	void JumpEnd();

	void LerpHp();

	void SetRightHand() { rightHand = GetTransformByNode(110) * world;}

public:
	Jean();
	~Jean();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void SetClip(AnimState state);

	void EndAttack();
};