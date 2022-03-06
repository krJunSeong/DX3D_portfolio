#pragma once

class InstancingChuchu
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

	CapsuleCollider* bodyCollider;
	BoxCollider* attackCollider;
	Bar* hpBar;

	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
	const float rotSpeed = 10.0f;
	const float movespeed = 10.0f;

	static int deathCount;

	bool isAttack;
	bool isMove;
	bool isJumping;
	bool isHit;

	ModelAnimatorInstancing* instancing;

	class Jean* player;
	Terrain* terrain;
	Quad* quad;

	void ActionIdle();
	void ActionTrace();
	void HpControl();

	void AttackEnd();
	void HitEnd();
	void DeathEnd();
public:
	InstancingChuchu(UINT instanceID);
	~InstancingChuchu();

	void Update();
	void Render();
	void GUIRender();
	void PostRender();

	void SpawnInit();
	void Damaged(float damage);

	void LerpHp();

	void SetEvents();

	Transform* GetTransform() { return transform; }
	Collider* GetCollider(){return bodyCollider;}
	int GetDeathCount(){return deathCount;}

	void SetRightHand(){rightHand = instancing->GetTransformByNode(instanceID, 110) * transform->GetWorld();};
	void SetClip(AnimState state, float speed = 1.0f);
	void SetInstancing(ModelAnimatorInstancing* instancing) { this->instancing = instancing; }
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetTransform(Transform* transform) { this->transform = transform; }
	void SetQuad(Quad* quad) {this->quad = quad;}
	void SetTarget(Jean* player){this->player = player;}
};