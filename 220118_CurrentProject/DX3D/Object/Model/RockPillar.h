#pragma once

class RockPillar : public Transform
{
private:
	CapsuleCollider* bodyCollider;
	Quad* quad;
	class RockShield* rockShield;

	Transform* objTransform;

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

	void SetTransform(Transform* transform) 
	{
		objTransform = transform;
		objTransform->SetParent(this);
		//objTransform->Load();
	}
	Transform* GetTransform(){return this;}
	Collider* GetCollider() { return bodyCollider; }
	float GetHp() {return hp;}

	void SetQuad(Quad* quad) { this->quad = quad; }
};