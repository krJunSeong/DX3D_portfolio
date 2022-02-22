#include "Framework.h"

Button::Button(wstring textureFile)
	: Quad(textureFile), state(NONE), Event(nullptr), IntEvent(nullptr),
	isDownCheck(false), intParam(0), VoidEvent(nullptr)
{
	collider = new BoxCollider();
	collider->SetParent(this);

	noneColor = Float4(1, 1, 1, 1);
	downColor = Float4(0.5f, 0.5f, 0.5f, 1.0f);
	overColor = Float4(0.9f, 0.9f, 0.9f, 1.0f);
}

Button::Button(wstring textureFile, Vector2 maxFrame, UINT frame)
	: Quad(textureFile, maxFrame, frame), state(NONE), Event(nullptr), IntEvent(nullptr),
	isDownCheck(false), intParam(0), VoidEvent(nullptr)
{
	//Vector2 size(Size().x / maxFrame.x, Size().y / maxFrame.y);
	collider = new BoxCollider();
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
	if (collider->RayCollision(mousePos))
	{
		if (KEY_DOWN(VK_LBUTTON))
			isDownCheck = true;

		if (KEY_PRESS(VK_LBUTTON))
			state = DOWN;
		else
			state = OVER;

		if (isDownCheck && KEY_UP(VK_LBUTTON))
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

		if (KEY_UP(VK_LBUTTON))
			isDownCheck = false;
	}

	switch (state)
	{
	case Button::NONE:
		colorBuffer->data.color = noneColor;
		break;
	case Button::DOWN:
		colorBuffer->data.color = downColor;
		break;
	case Button::OVER:
		colorBuffer->data.color = overColor;
		break;	
	}

	collider->UpdateWorld();
	UpdateWorld();
}

void Button::Render()
{
	Quad::Render();
	collider->Render();
	Font::Get()->RenderText(text, "default", GlobalPos(), Size());
}

