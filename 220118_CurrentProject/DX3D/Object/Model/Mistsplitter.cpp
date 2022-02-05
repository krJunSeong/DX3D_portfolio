#include "framework.h"

Mistsplitter::Mistsplitter(Matrix* rightHand)
	: Model("Mistsplitter")
{
    SetParent(rightHand);
    Transform::Load();

    collider = new BoxCollider(Vector3(3,3,3));
    collider->tag = "MistsplitterCollider";
    collider->SetParent(this);
    collider->Load();
    collider->isActive = true;
}

Mistsplitter::~Mistsplitter()
{
    delete collider;
}

void Mistsplitter::Update()
{
    Transform::UpdateWorld();
    collider->UpdateWorld();
}

void Mistsplitter::Render()
{
    Model::Render();

    collider->Render();
}
