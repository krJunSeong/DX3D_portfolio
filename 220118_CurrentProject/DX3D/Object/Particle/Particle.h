#pragma once

class Particle
{
protected:
	const UINT MAX_COUNT = 1000;

	bool isActive = false;
	Vector3 position;

	Material* material;
	VertexBuffer* vertexBuffer;

	GeometryShader* geometryShader;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

	UINT particleCount = 0;	// 최대개수

public:
	Particle();
	~Particle();

	virtual void Update(){};
	virtual void Render();
	virtual void GUIRender(){};

	virtual void Play(Vector3 position);

	void Stop();
};