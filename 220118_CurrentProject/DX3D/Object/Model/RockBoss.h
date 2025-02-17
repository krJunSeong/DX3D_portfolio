#pragma once

class RockBoss
{
private:
	enum AnimState
	{
		IDLE, ATTACK
	}state;

	UINT instanceID;

	// Class Instance
	vector<RockPillar*> rockPillares;
	RockShield* rockShield;
	ModelInstancing* instancing;			// rockPillar
	ModelInstancing* rockShieldInstancing;	// rockShield

	Bar* hpBar;
	Particle* effect;

	Vector3 originPlayerPosDir;
	bool isCrash = false;

private:
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
	const float rotSpeed = 10.0f;
	const float movespeed = 10.0f;

	const float rockShieldDistance = 100.0f;
	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isHit;

	class Jean* player;
	Terrain* land;
private:
	// phase1 Data
	const float FloatingDuration = 3.0f;
	const float LimitPlayTime = 5.0f;
	const float LimitIdleTime = 5.0f;
	const float MoveSpeed = 30.0f;
	const float SpinSpeed = 5.0f;
	const float Att = 30.0f;
	float floatingSpeed = 2.0f;;
	float floatingTime = 0.0f;
	float idleTime = 0.0f;
	bool isPhase1 = true;

private:
	// phase2 Data
	const float Phase2Hp = 30.0f;
	const float LimitInitTime = 10.0f;
	const float LimitAttackTime = 15.0f; // ���ѽð�
	float attackTime = 0.0f;
	float initTime = 5.0f;
	bool isPillaresInit = false;
	bool isIniting = false;
	bool isPhase2 = false;

	void InitRockPillares();

	void CreateObject();
	void PhaseCheck();
public:
	RockBoss();
	~RockBoss();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Collision(Collider* collider, float damage);

	void Phase1();
	void Phase1Attack();
	void Floating();

	void Phase2();

	//void SetInstancing(ModelInstancing* instancing) { this->rockPillar = instancing; }
	void SetTerrain(Terrain* terrain) { this->land = terrain; }
	void SetPlayer(Jean* jean) { player = jean; }
	bool GetIsPhase2(){return isPhase2;}
	//void SetQuad(Quad* land) { quad = land; }

	vector<RockPillar*> GetRockPillares(){return rockPillares;}
	RockShield* GetRcokShield() {return rockShield;}

	void SetBossPosition(Vector3 pos){rockShield->position = pos;}
};