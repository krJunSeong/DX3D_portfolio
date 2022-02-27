#include "Framework.h"

Button::Button(wstring textureFile)
	: state(NONE), Event(nullptr), IntEvent(nullptr),
	isDownCheck(false), intParam(0), VoidEvent(nullptr)
{
	tag = "Button";
	GetMaterial()->SetShader(L"Texture.hlsl");
	GetMaterial()->SetDiffuseMap(textureFile);
	SetParent(this);

	collider = new BoxCollider();
	collider->tag = "ButtonCollider";
	collider->SetParent(this);

	noneColor = Float4(1, 1, 1, 1);
	downColor = Float4(0.5f, 0.5f, 0.5f, 1.0f);
	overColor = Float4(0.9f, 0.9f, 0.9f, 1.0f);
}

Button::~Button()
{
	delete collider;
}

void Button::Update()
{
	// isPointColision
	if (collider->IsPointCollision({mousePos.x, mousePos.y}))
	{
		if (MOUSE_CLICK(0))
			isDownCheck = true;

		if (MOUSE_PRESS(0))
			state = DOWN;
		else
			state = OVER;

		if (isDownCheck && MOUSE_UP(0))
		{
			if (Event != nullptr)
				Event();

			if (IntEvent != nullptr)
				IntEvent(intParam);

			if (VoidEvent != nullptr)
				VoidEvent(voidParam);

			isDownCheck = false;
		}
	}
	else
	{
		state = NONE;

		if (MOUSE_UP(0))
			isDownCheck = false;
	}

	switch (state)
	{
	case Button::NONE:
		Quad::SetColor(noneColor);
		collider->SetColor(noneColor);
		break;
	case Button::DOWN:
		Quad::SetColor(downColor);
		collider->SetColor(downColor);
		break;
	case Button::OVER:
		Quad::SetColor(overColor);
		collider->SetColor(overColor);
		break;	
	}

	collider->UpdateWorld();
	UpdateWorld();
}

void Button::Render()
{
	Quad::Render();
	collider->Render();
	//Font::Get()->RenderText(text, "default", GlobalPos(), Size());
}

void Button::GUIRender()
{
	Transform::GUIRender();
	collider->GUIRender();
	GetMaterial()->GUIRender();
}

