#include "Framework.h"

Item::Item(wstring textureFile, Vector2 maxFrame, ItemData data)
	: data(data), isSelected(false)
{
	button = new Button(textureFile, maxFrame, data.image);
	button->SetParent(&matrix);
	button->GetCollider()->scale *= 1.5f;

	selected = new Quad(L"Textures/UI/selected_.png");
	selected->SetParent(&matrix);
	selected->position.y = -50;

	equit = new Quad(L"Textures/UI/E.png");
	equit->SetParent(&matrix);
	equit->position += {15, 10};
	equit->isActive = false;
}

Item::~Item()
{
	delete equit;
	delete button;
	delete selected;
}

void Item::Update()
{
	equit->UpdateWorld();
	button->Update();
	selected->UpdateWorld();
	UpdateWorld();
}
//1240590
void Item::Render()
{
	button->Render();
	if(isSelected)
		selected->Render();
	equit->Render();
}
