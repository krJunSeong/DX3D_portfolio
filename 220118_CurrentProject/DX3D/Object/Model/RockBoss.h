#pragma once

class RockBoss
{
private:
	enum AnimState
	{
		IDLE, RUN, ATTACK, HIT, DEATH
	} animState;

	UINT instanceID;

	Matrix rightHand;

	vector<RockPillar*> rockPillares;
	RockShield* rockShield;
	ModelInstancing* instancing;
	ModelInstancing* rockShieldInstancing;

	Bar* hpBar;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
	const float rotSpeed = 10.0f;
	const float movespeed = 10.0f;
	
	const float rockShieldDistance = 40.0f;
	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isHit;

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
	void Collision(Collider* collider, float damage);
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

	void LerpHp();

	void Collision();
};