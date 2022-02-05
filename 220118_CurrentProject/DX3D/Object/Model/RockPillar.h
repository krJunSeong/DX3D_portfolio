#pragma once

class RockPillar : public ModelInstancing
{
private:
	CapsuleCollider* bodyCollider;
	Transform* transform;
	Quad* quad;
	class RockShield* rockShield;
	
	// 이관할 것
	//vector<CapsuleCollider*> bodyCollideres;

	Bar* hpBar;
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
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

	void Init();
	void HpControll();
	void LerpHp();

	Transform* GetTransform() { return transform; }
	Collider* GetCollider() { return bodyCollider; }

	void SetTransform(Transform* tr){ transform = tr;}
	void SetQuad(Quad* quad) { this->quad = quad; }
};