#include "Framework.h"

Inventory::Inventory()
//L"Textures/UI/pannel_bg.png"
	: UIPanel(L"Textures/UI/pannel_bg.png"), gold(10000)
{

}

Inventory::~Inventory()
{
	for (Item* item : items)
		delete item;
}

void Inventory::Update()
{
	if (!isActive) return;

	UIPanel::Update();

	for (Item* item : items)
	{
		item->Update();

		// Equit
		if (KEY_DOWN(VK_LBUTTON))
		{
			if (item->GetButton()->GetCollider()->IsPointCollision(mousePos))
				item->GetEquit()->isActive = !(item->GetEquit()->isActive);
		}
	}
}

void Inventory::Render()
{
	if (!isActive) return;

	UIPanel::Render();

	for (Item* item : items)
		item->Render();

	string str = "GOLD : " + to_string(gold);
	Vector2 pos = GlobalPos();
	//pos.y += 200;
	Font::Get()->RenderText(str, "item", pos);
}

void Inventory::BuyItem(ItemData item)
{
	if (gold < item.price)
		return;

	gold -= item.price;

	Vector2 startOffset(-150, -50);

	UINT colCount = 7;

	Item* newItem = new Item(L"Textures/TalesStoreItem.png", Vector2(4, 1), item);
	newItem->SetParent(&matrix);

	Vector2 size(50, 50);

	newItem->position.x = startOffset.x + size.x * (items.size() % colCount);
	newItem->position.y = startOffset.y + size.y * (items.size() / colCount);

	items.push_back(newItem);
}

void Inventory::ExitStore()
{
	for (Item* item : items)
	{
		if (item->GetEquit()->isActive)
		{
			DataManager::Get()->AddplayerEquit(
			item->GetData().attack,
			item->GetData().defence);
		}
	}
}
