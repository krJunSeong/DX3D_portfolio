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

	// --------------- Weapon --------------- 
	Mistsplitter* mistsplitter;
	Model* zephyrus;
	Wing* wing;
	vector<InstancingChuchu*> chuchus;
	class RockBoss* boss;

	// ----------------- Status -----------------
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float moveSpeed;
	float rotSpeed;
	float jumpPower;
	float gravity;
	float att = 20.0f;

	Bar* hpBar;
	class SkillBall* ball;
	Quad* skillUI;

	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isAirFloating;
	bool isHit = false;
private:
	void Init();
	void Move();
	void Attack();
	
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

	void Damaged(float damage);
	void HitEnd();

	Collider* GetCollider() { return bodyCollider;}

	void SetClip(AnimState state);
	void SetRockBoss(RockBoss* boss) {this->boss = boss;}

	void EndAttack();
};