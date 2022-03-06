#pragma once

class RockPillar : public Transform
{
private:
	static int PillarCount;
	CapsuleCollider* bodyCollider;
	Quad* quad;
	Terrain* land;
	class RockShield* rockShield;

	Transform* objTransform;

	// ------------ Damage ----------------
	bool isHit = false;
	float hitTime = 0.0f;
	const float LimitHitTime = 1.3f;

	Bar* hpBar;
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;

	bool isIniting = false;
	bool isEffect = false;
public:
	RockPillar();
	~RockPillar();

	void CreateObject();
	void SetParent(Transform* trf);
	void SetParent(RockShield* target) {rockShield = target;}

	void Update();
	void Render();
	void GUIRender();
	void PostRender();

	void Damaged(float damage);

	void Init(Terrain* terrain);
	void HpControll();
	void LerpHp();

	void SetTransform(Transform* transform) 
	{
		objTransform = transform;
		objTransform->SetParent(this);
		//objTransform->Load();
	}
	Transform* GetTransform(){return this;}
	Collider* GetCollider() { return bodyCollider; }

	void SetQuad(Quad* quad) { this->quad = quad; }
	void SetTerrain(Terrain* terrain) { land = terrain;}

	float GetHp() { return hp; }
	static int GetPillarCount() {return PillarCount;}
};
