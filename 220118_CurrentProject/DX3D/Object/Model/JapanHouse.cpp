#include "framework.h"

JapanHouse::JapanHouse()
	: Model("JapanHouse")
{
	//Transform::Load();
	// collider = new BoxCollider();
}

JapanHouse::~JapanHouse()
{
	//delete collider;
}

void JapanHouse::Update()
{
	Transform::UpdateWorld();
	//collider->UpdateWorld();
}

void JapanHouse::Render()
{
	Model::Render();

	//collider->Render();
}
