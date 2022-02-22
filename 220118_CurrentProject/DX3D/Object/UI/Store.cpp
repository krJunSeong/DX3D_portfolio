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

	// 장비설명
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

		//items.size() = 들어간 아이템 사이즈
		// 시작위치 + 50 * 들어간 수 1 % 4.. 0 1 2 3   0 1 2 3
		// 시작위치 + 50 * 들어간 수 1 / 4.. 0 0 0 0   1 1 1 1 (int값이라 소수점제거)
		item->position.x = startOffset.x + size.x * (items.size() % colCount);
		item->position.y = startOffset.y + size.y * (items.size() / colCount);

		item->GetButton()->SetVoidEvent(bind(&Store::SelectItem, this, placeholders::_1));
		item->GetButton()->SetVoidParam(item);

		items.push_back(item);		
	}	
}

void Store::SelectItem(void* item)
{
	selectItem = (Item*)item;			// 선택한 아이템을 갖고 온다

	for (Item* item : items)			// 아이템 리스트를 뒤진다
	{
		if (selectItem == item)			// 찾으면 함수호출 true
			item->SetSelected(true);	// 선택 UI 띄우기
		else
			item->SetSelected(false);	// 아니면 false
	}

	// Item Text print
	ItemData data = selectItem->GetData();	// 찾은 아이템의 데이터를 갖고 온다.
	itemInfoText = "";						// 아이템 정보값의 텍스트를 저장한단.
	itemInfoText += data.name + "\n" +
					"ATK " + to_string(data.attack) + "\t" +
					"DEF " + to_string(data.defence) + "\t" +
					"Price " + to_string(data.price) + "G" + "\n\n" +
					data.info;
		
	//itemInfoText += ;
}

void Store::BuyItem()
{	// Scene에서 store->SetInventory(inventory); 로 인벤토리를 지정한다.
	inventory->BuyItem(selectItem->GetData());	// 인벤터로의 BuyItem(산 아이템의 정보) 넘겨준다.
}
