#pragma once

class ReportScene : public Scene
{
public:
	ReportScene();
	~ReportScene();

private:
	Jean* jean;
	JapanHouse* house;
	RockBoss* boss;
	Terrain* land;
	Sky* sky;

	Wall* leftWall;
	Wall* upWall;
	Wall* rightWall;
	Wall* downWall;

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

	virtual void Start() override;
	virtual void End() override;
};