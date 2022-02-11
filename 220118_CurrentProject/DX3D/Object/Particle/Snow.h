#pragma once

class Snow : public Particle
{
private:
	struct VertexSnow
	{
		Float3 position;
		Float2 size;
		Float2 random;
	};

	WeatherBuffer* buffer;
	VertexSnow* vertices;

public:
	Snow();
	~Snow();

	void Update();
	void Render();
	void GUIRender();

private:
	void Create();
};
