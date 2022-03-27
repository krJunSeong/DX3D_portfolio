#pragma once

class Bar : public Transform
{
private:
	ValueBuffer* valueBuffer;

	Quad* frontBar;		// 앞면 채울 이미지
	Quad* backBar;		// 백 기본 이미지

	float maxValue;		// 최대값
	float curValue;		// 현재값
	float lerpValue;	// 러프값

public:
	Bar(wstring frontImage, wstring backImage, float maxValue = 100.0f);
	~Bar();

	void Update();
	void Render();

	void SetValue(float value) { curValue = value; }
	void SetLerpValue(float value) { lerpValue = value; }
};