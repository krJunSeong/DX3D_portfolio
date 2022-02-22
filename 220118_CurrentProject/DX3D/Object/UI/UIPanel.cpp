#include "Framework.h"

UIPanel::UIPanel(wstring textureFile) : isDrag(false)
{
	panel = new Quad();
	panel->GetMaterial()->SetShader(L"Texture.hlsl");
	panel->GetMaterial()->SetDiffuseMap(textureFile);
	panel->SetParent(this);

	// tag name이 먹히지 않고 있다.
	//panel->Load();

	//infoPanel->SetParent(&matrix);

	collider = new BoxCollider({1,1, 0});
	collider->SetParent(this);
}

UIPanel::~UIPanel()
{
	delete panel;
	delete collider;
}

void UIPanel::Update()
{
	if (MOUSE_PRESS(0))
	{
		//Vector2 temp = { mousePos.x, WIN_HEIGHT - mousePos.y };

		if(collider->IsPointCollision({mousePos.x, mousePos.y}))
		{
			//offset = mousePos - position;
			isDrag = true;
		}

		if (isDrag)
		{
			Vector2 temp = { mousePos.x, WIN_HEIGHT - mousePos.y };
			position.x = temp.x;// + offset.x;
			position.y = temp.y;// + offset.y;
		}
	}
	else
		isDrag = false;

	UpdateWorld();
	panel->UpdateWorld();
	collider->UpdateWorld();
}

void UIPanel::Render()
{
	panel->Render();
	collider->Render();
}

void UIPanel::GUIRender()
{
	Transform::GUIRender();
	collider->GUIRender();
}


/*
if (KEY_DOWN(VK_LBUTTON))
{
	if (collider->IsPointCollision(mousePos))
	{
		offset = mousePos - position;
		isDrag = true;
	}
	else
	{
		isDrag = false;
	}
}*/