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
	if(MOUSE_CLICK(0))  //if (MOUSE_PRESS(0))
	{
		if(collider->IsPointCollision({mousePos.x, mousePos.y}))
		{
			Vector3 tempMouse = {mousePos.x, WIN_HEIGHT - mousePos.y, mousePos.z };
			offset = tempMouse - position;
			isDrag = true;
		}

		else isDrag = false;
	}

	// press 누르고 있으면
	if (isDrag && MOUSE_PRESS(0)) // 마우스 콜라이더 처리되고, press 되면 이동
	{
		//Vector2 temp = { mousePos.x, WIN_HEIGHT - mousePos.y };
		position.x = mousePos.x - offset.x;
		position.y = WIN_HEIGHT - mousePos.y - offset.y;
	}

	if(MOUSE_UP(0)) isDrag = false;
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