#pragma once

class Inventory : public UIPanel
{
private:
	UINT gold;
	vector<Item*> items;

public:
	Inventory();
	~Inventory();

	void Update();
	void Render();

	void BuyItem(ItemData item);
	void ExitStore();
};