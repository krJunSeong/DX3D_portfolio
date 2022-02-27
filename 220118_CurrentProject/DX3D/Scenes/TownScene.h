#pragma once

class TownScene : public Scene
{
private:
	Model* Siro_Stagenum_008_1;

public:
	TownScene();
	~TownScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

	virtual void Start() override;
	virtual void End() override;

};