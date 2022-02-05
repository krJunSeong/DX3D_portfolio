#pragma once

class RockShield : public ModelInstancing
{
private:
	SphereCollider* bodyCollider;

public:
	RockShield(int count = 1);
	~RockShield();

	void CreateObject(int count);

	void Update();
	void Render();
};