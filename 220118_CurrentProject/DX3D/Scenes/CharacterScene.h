#pragma once

class  CharacterScene : public Scene
{
private:
	Zombie*  zombie;
	Terrain* terrain;
	Horse*   horse;
	Gumdramon* gumdramon;
	Chuchu* chuchu;

	Sky* sky;
	Quad* quad;
public:
	CharacterScene();
	~CharacterScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
