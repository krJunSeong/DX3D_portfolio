#include "framework.h"

Wing::Wing()
	: Model("Wing")
{
	Transform::Load();
	isActive = false;
}

Wing::~Wing()
{
}

void Wing::Update()
{
	UpdateWorld();
}

void Wing::Render()
{
	if(!isActive) return;

	Model::Render();
}