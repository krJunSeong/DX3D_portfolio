#pragma once

class UIPanel : public Transform
{
protected:
	Quad* panel;

	BoxCollider* collider;
	
	Vector3 offset;
	bool isDrag;

public:
	UIPanel(wstring textureFile = L"Textures/UI/pannel_bg.png");
	virtual ~UIPanel();

	void Update();
	void Render();
	void GUIRender();

	//void SetinfoPanel(bool b) { infoPanel->isActive = b;}
};