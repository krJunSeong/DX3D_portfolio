#pragma once

class Rain : public Particle
{
private:
	WeatherBuffer* buffer;
	VertexSize* vertices;

public:
	Rain();
	~Rain();

	void Update();
	void Render();
	void GUIRender();

private:
	void Create();

};