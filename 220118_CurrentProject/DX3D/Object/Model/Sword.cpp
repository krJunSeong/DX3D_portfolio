#include "Framework.h"

Sword::Sword()
    :Model("sword")
{
    collider = new BoxCollider();
    collider->tag = "SwordCollider";
    collider->SetParent(this);
    collider->Load();
    collider->isActive = true;
}

Sword::~Sword()
{
    delete collider;
}

void Sword::Update()
{
    UpdateWorld();
    collider->UpdateWorld();
}

void Sword::Render()
{
    Model::Render();

    collider->Render();
}