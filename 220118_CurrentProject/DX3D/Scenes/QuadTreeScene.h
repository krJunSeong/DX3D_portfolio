#pragma once

class QuadTreeScene : public Scene
{
public:
	QuadTreeScene();
	~QuadTreeScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
