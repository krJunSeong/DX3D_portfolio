#pragma once

class Inventory;

class Store : public UIPanel
{
private:
	Quad* infoPanel;

	vector<Item*> items;
	Item* selectItem;

	string itemInfoText;

	Button* buyButton;

	Inventory* inventory;
public:
	Store();
	~Store();

	void Update();
	void Render();
	
	void SetInventory(Inventory* inventory) { this->inventory = inventory; }
	void SetActive(bool ac) {isActive = ac;}
private:
	void LoadItems();
	void SelectItem(void* item);

	void BuyItem();
};