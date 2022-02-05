#pragma once

class AstarScene : public Scene
{
private:
	Terrain* terrain;

	AStar* aStar;

	vector<Box*> boxes;
public:
	AstarScene();
	~AstarScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateBoxes();
};

/*
	Terrain으로 노드를 깐다
*/