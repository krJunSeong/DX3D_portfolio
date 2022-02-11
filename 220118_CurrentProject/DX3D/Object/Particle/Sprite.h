#pragma once

class Sprite : public Particle
{
private:
	 //maxframe, curFrame에 넘겨줘야 하니 글로벌 버퍼로 가자
	SpriteBuffer* buffer;
	VertexSize* vertices;

	float time;
	float speed;
	UINT curFrameCount;	// 스프라이트: 2x2 배열형태, 돌릴 떄는 1차원 0 ~ 1
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
