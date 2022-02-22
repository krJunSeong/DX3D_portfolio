#pragma once

class Item : public Transform
{
private:
	ItemData data;

	Button* button;
	Quad* selected;
	Quad* equit;

	bool isSelected;

public:
	Item(wstring textureFile, Vector2 maxFrame, ItemData data);
	~Item();

	void Update();
	void Render();

	void SetSelected(bool value) { isSelected = value; }
	Button* GetButton() { return button; }
	Quad* GetEquit() {return equit;}
	ItemData GetData() { return data; }
};