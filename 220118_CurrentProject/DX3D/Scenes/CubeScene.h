#pragma once

class CubeScene : public Scene
{
private:
	Cube* cube;
	Cube* cube2;

	ReportRobot_211201* robot;

	bool isDown;
public:
	CubeScene();
	~CubeScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
