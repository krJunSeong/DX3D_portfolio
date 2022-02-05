#pragma once

class TerrainScene : public Scene
{
private:
	Terrain* terrain;
	ReportRobot_211201* robot;
	Cube* cube;

public:
	TerrainScene();
	~TerrainScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
