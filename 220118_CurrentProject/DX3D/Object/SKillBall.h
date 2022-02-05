#pragma once

class SkillBall : public Transform
{
public:
	SkillBall(Vector3 pos = {0,0,0});
	~SkillBall();

	void Update();
	void Render();

	void Fire();

	void SetTarget(Transform* trs){target = trs;};

	WorldBuffer* worldBuffer;
private:
	Sphere* ball;
	Collider* collider;
	Transform* target;

	Vector3 tempFs;
	float limitTime;
};