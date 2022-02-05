#pragma once

class Bar : public Transform
{
private:
	ValueBuffer* valueBuffer;

	Quad* frontBar;
	Quad* backBar;

	float maxValue;
	float curValue;
	float lerpValue;

public:
	Bar(wstring frontImage, wstring backImage, float maxValue = 100.0f);
	~Bar();

	void Update();
	void Render();

	void SetValue(float value) { curValue = value; }
	void SetLerpValue(float value) { lerpValue = value; }
};