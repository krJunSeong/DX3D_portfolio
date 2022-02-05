#include "framework.h"

Box::Box(string tag)
{
	this->tag = tag;

	Transform::Load();
	material->SetDiffuseMap(L"Textures/Landscape/Box.png");

	collider = new BoxCollider();
	collider->SetParent(this);
	collider->tag = tag + "Collider";
	collider->Load();
}

Box::~Box()
{
	delete collider;
}

void Box::Update()
{
	Transform::UpdateWorld();
	collider->UpdateWorld();
}

void Box::Render()
{
	Cube::Render();
	collider->Render();
}

void Box::GUIRender()
{
	Transform::GUIRender();
	collider->GUIRender();
}