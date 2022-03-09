#pragma once

class ReportScene : public Scene
{


private:
	map<UINT, string> data;

	Jean* jean;
	JapanHouse* house;
	RockBoss* boss;
	Terrain* land;
	Sky* sky;
	Quad* backLand;

	// TODO:: 1. 화면 레인 작업 2. 몬스터 -> boss 페이즈 넘김 작성
	Particle* rain;

	// --------------------- pos ---------------------
private:
	BoxCollider* startPos;
	BoxCollider* bossStartPos;

	// --------------------- Wall ---------------------
private:
	Wall* leftWall;
	Wall* upWall;
	Wall* rightWall;
	Wall* downWall;

private:
	bool isPhase2Setting = false;
	bool isBossPhase = false;

	void Phase2MapSetting();

public:
	ReportScene();
	~ReportScene();

private:
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