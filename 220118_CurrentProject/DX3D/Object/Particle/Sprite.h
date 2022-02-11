#pragma once

class Sprite : public Particle
{
private:
	 //maxframe, curFrame�� �Ѱ���� �ϴ� �۷ι� ���۷� ����
	SpriteBuffer* buffer;
	VertexSize* vertices;

	float time;
	float speed;
	UINT curFrameCount;	// ��������Ʈ: 2x2 �迭����, ���� ���� 1���� 0 ~ 1
	UINT maxFrameCount;

	Float2 size;

public:
	Sprite(wstring imageFile, Float2 maxFrame, bool isAdditive = false);
	~Sprite();

	void Update();
	void Render();
	void GUIRender();

	void Play(Vector3 position);

private:
	void Create();
};
