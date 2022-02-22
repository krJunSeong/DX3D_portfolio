#include "Framework.h"

Store::Store() : UIPanel(L"Textures/UI/pannel_bg.png")
{
	LoadItems();

	Font::Get()->Add("item", L"dohuyn", Float3(0, 0, 0), 20);

	infoPanel = new Quad(L"Textures/UI/info_bg.png");
	infoPanel->position = { WIN_CENTER_X, WIN_CENTER_Y * 1.65 };

	//"Purchase" Button 
	buyButton = new Button(L"Textures/UI/button.png");
	buyButton->SetText(L"Purchase");
	buyButton->SetParent(&matrix);
	buyButton->position.x += 100;
	buyButton->position.y += 130;
	buyButton->SetEvent(bind(&Store::BuyItem, this));
}

Store::~Store()
{
	for (Item* item : items)
		delete item;

	delete buyButton;
	delete infoPanel;
}

void Store::Update()
{
	if (!isActive) return;

	UIPanel::Update();

	for (Item* item : items)
		item->Update();	

	buyButton->Update();
	infoPanel->UpdateWorld();
}

void Store::Render()
{
	if (!isActive) return;

	UIPanel::Render();
	infoPanel->Render();

	for (Item* item : items)
		item->Render();

	buyButton->Render();

	// ��񼳸�
	Vector2 infoPos = {WIN_CENTER_X, WIN_CENTER_Y * 1.5};
	//infoPos.y += 200;

	Font::Get()->RenderText(itemInfoText, "item", infoPos);
}

void Store::LoadItems()
{
	Vector2 startOffset(-150, -50);

	UINT colCount = 4;
	
	for (auto data : DataManager::Get()->GetItemData())
	{		
		Item* item = new Item(L"Textures/TalesStoreItem.png", Vector2(4, 1), data.second);
		item->SetParent(&matrix);

		//Vector2 size(150, 150);
		Vector2 size(50, 50);

		//items.size() = �� ������ ������
		// ������ġ + 50 * �� �� 1 % 4.. 0 1 2 3   0 1 2 3
		// ������ġ + 50 * �� �� 1 / 4.. 0 0 0 0   1 1 1 1 (int���̶� �Ҽ�������)
		item->position.x = startOffset.x + size.x * (items.size() % colCount);
		item->position.y = startOffset.y + size.y * (items.size() / colCount);

		item->GetButton()->SetVoidEvent(bind(&Store::SelectItem, this, placeholders::_1));
		item->GetButton()->SetVoidParam(item);

		items.push_back(item);		
	}	
}

void Store::SelectItem(void* item)
{
	selectItem = (Item*)item;			// ������ �������� ���� �´�

	for (Item* item : items)			// ������ ����Ʈ�� ������
	{
		if (selectItem == item)			// ã���� �Լ�ȣ�� true
			item->SetSelected(true);	// ���� UI ����
		else
			item->SetSelected(false);	// �ƴϸ� false
	}

	// Item Text print
	ItemData data = selectItem->GetData();	// ã�� �������� �����͸� ���� �´�.
	itemInfoText = "";						// ������ �������� �ؽ�Ʈ�� �����Ѵ�.
	itemInfoText += data.name + "\n" +
					"ATK " + to_string(data.attack) + "\t" +
					"DEF " + to_string(data.defence) + "\t" +
					"Price " + to_string(data.price) + "G" + "\n\n" +
					data.info;
		
	//itemInfoText += ;
}

void Store::BuyItem()
{	// Scene���� store->SetInventory(inventory); �� �κ��丮�� �����Ѵ�.
	inventory->BuyItem(selectItem->GetData());	// �κ��ͷ��� BuyItem(�� �������� ����) �Ѱ��ش�.
}
