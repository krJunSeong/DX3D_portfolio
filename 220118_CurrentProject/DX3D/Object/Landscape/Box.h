#pragma once

class Box : public Cube
{
private:
	BoxCollider* collider;

public:
	Box(string tag);
	~Box();

	void Update();
	void Render();
	void GUIRender();

	BoxCollider* GetCollider() {return collider;}
};