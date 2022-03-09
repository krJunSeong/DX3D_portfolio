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

	// TODO:: 1. ȭ�� ���� �۾� 2. ���� -> boss ������ �ѱ� �ۼ�
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
	// Scene��(��) ���� ��ӵ�
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