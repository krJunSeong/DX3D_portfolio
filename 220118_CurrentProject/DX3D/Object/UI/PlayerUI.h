#pragma once

class PlayerUI
{
private:
	UIPanel* ui;
	Button* button;
	Quad* icon;

	Bar* hpBar;
	Bar* staminaBar;

	float hp = 200.0f;
	float lerpHp = 200.0f;
	float lerpSpeed = 1.0f;

	float stamina = 100.0f;
	float lerpStamina = 100.0f;

public:
	PlayerUI();
	~PlayerUI();

	void Update();
	void Render();
	void GUIRender();

	// --------------------- ÇÔ¼ö -------------
	void CreateUI();

	// -------------- Get Set ----------------
	Bar* GetHpbar(){ return hpBar;}
	Bar* GetStaminaBar() { return staminaBar; }

	float GetHp(){return hp;}
	float GetStamina(){return stamina;}

	void SetHp(float value) {hp = value;}
	void SetStamina(float value) { stamina = value; }
};