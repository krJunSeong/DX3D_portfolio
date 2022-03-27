#pragma once

class TestScene : public Scene
{
private:
	UIPanel* ui;
	Button* button;
	Quad* icon;

	ParticleSystem* particleSystem;
	Particle* particle;
	Bar* hpBar;
	Bar* staminaBar;

	Jean* jean;

	float hp = 200.0f;
	float lerpHp = 200.0f;
	float lerpSpeed = 1.0f;

	float stamina = 100.0f;
	float lerpStamina = 100.0f;

	Vector3 tempPos = {};
public:
	TestScene();
	~TestScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
