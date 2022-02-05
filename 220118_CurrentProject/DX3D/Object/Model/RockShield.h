#pragma once

class RockShield : public ModelInstancing
{
private:
	SphereCollider* bodyCollider;

public:
	RockShield();
	~RockShield();

	void CreateObject();

	void Update();
	void Render();
	void GUIRender();
};