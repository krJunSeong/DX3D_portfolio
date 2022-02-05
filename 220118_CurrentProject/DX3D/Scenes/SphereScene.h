#pragma once

class SphereScene : public Scene
{
private:
	Sphere* sphere;
	Cube* cube;

public:
	SphereScene();
	~SphereScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};