#pragma once

class RockShield : public Transform
{
private:
	SphereCollider* bodyCollider;
	Transform* objTransform;

	const float FloatingDuration = 3.0f;
	float floatingTime;
	float floatingSpeed;

public:
	RockShield();
	~RockShield();

	void CreateObject();

	void Update();
	void Render();
	void GUIRender();

	void Floating();

	void SetTransform(Transform* transform)
	{
		objTransform = transform;
		objTransform->SetParent(this);
		//objTransform->Load();
	}
};