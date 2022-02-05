#pragma once

class RockPillar : public ModelInstancing
{
private:
	vector<CapsuleCollider*> bodyCollideres;

public:
	RockPillar(int count = 1);
	~RockPillar();

	void CreateObject(int count);
	void SetParent(Transform* trf);

	void Update();
	void Render();
};