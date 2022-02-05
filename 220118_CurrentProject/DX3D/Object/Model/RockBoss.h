#pragma once

class RockBoss
{
private:
	enum AnimState
	{
		IDLE, RUN, ATTACK, HIT, DEATH
	} animState;

	enum class ActionState
	{
		IDLE, TRACE, ATTACK
	} actionState;

	UINT instanceID;

	Matrix rightHand;

	Transform* transform;

	RockPillar* rockPillar;
	CapsuleCollider* rockShieldColliders;

	BoxCollider* attackCollider;
	Bar* hpBar;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
	const float rotSpeed = 10.0f;
	const float movespeed = 10.0f;

	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isHit;

	ModelInstancing* rockShield;

	class Jean* player;
	Terrain* terrain;
	Quad* quad;

	void ActionIdle();
	void ActionTrace();
	void HpControl();

	void AttackEnd();
	void HitEnd();
	void DeathEnd();

	void CreateObject();
public:
	RockBoss();
	~RockBoss();

	void Update();
	void Render();
	void PostRender();

	void GUIRender();
	void Damage();

	//void SetInstancing(ModelInstancing* instancing) { this->rockPillar = instancing; }
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetTransform(Transform* transform) { this->transform = transform; }

	void LerpHp();

	void Collision();
};