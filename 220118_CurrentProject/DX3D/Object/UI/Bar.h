#pragma once

class Bar : public Transform
{
private:
	ValueBuffer* valueBuffer;

	Quad* frontBar;		// �ո� ä�� �̹���
	Quad* backBar;		// �� �⺻ �̹���

	float maxValue;		// �ִ밪
	float curValue;		// ���簪
	float lerpValue;	// ������

public:
	Bar(wstring frontImage, wstring backImage, float maxValue = 100.0f);
	~Bar();

	void Update();
	void Render();

	void SetValue(float value) { curValue = value; }
	void SetLerpValue(float value) { lerpValue = value; }
};