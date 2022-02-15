#include "framework.h"

Wall::Wall()
	: Model("Wall")
{
	Transform::Load();
	isActive = true;
}

Wall::~Wall()
{
}

void Wall::Update()
{	
	UpdateWorld();
}

void Wall::Render()
{
	if(!isActive) return;

	Model::Render();
}

void Wall::GUIRender()
{
	Transform::GUIRender();
}
