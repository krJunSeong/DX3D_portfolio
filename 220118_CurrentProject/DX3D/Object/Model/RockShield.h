#pragma once

class RockShield : public Transform
{
private:
	SphereCollider* bodyCollider;
	Transform* objTransform;

	const float FloatingDuration = 3.0f;
	float floatingTime;
	float floatingSpeed;
	bool isHit = false;
	float hitTime = 0.0f;
	const float LimitHitTime = 1.3f;

	Bar* hpBar;
	float hp = 100.0f;
	float lerpHp = 100.0f;
	float lerpSpeed = 1.0f;
	float hpBarScaleRate = 20.0f;
public:
	RockShield();
	~RockShield();

	void CreateObject();

	void Update();
	void Render();
	void GUIRender();
	void PostRender();

	void HpControll();
	void LerpHp();
	void Damaged(float damage);

	void Floating();

	void SetTransform(Transform* transform)
	{
		objTransform = transform;
		objTransform->SetParent(this);
		//objTransform->Load();
	}

	Collider* GetCollider(){return bodyCollider;}
	Bar* GetHpBar() { return hpBar;}
	float GetHp(){return hp;}
};