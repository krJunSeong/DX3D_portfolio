#pragma once

class ReportScene : public Scene
{
public:
	ReportScene();
	~ReportScene();

private:
	map<UINT, string> data;

	Jean* jean;
	JapanHouse* house;
	RockBoss* boss;
	Terrain* land;
	Sky* sky;
	Quad* backLand;

	// --------------------- pos ---------------------
	BoxCollider* startPos;
	BoxCollider* bossStartPos;

	// --------------------- Wall ---------------------
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

private:
	void LightContorll();
};