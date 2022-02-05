#include "Framework.h"

bool Collider::isVisible = true;

Collider::Collider()
{
    material = new Material(L"Collider.hlsl");
    worldBuffer = new MatrixBuffer();

    SetColor(0, 1, 0);
}

Collider::~Collider()
{
    delete material;
    delete mesh;
    delete worldBuffer;
}

void Collider::Render()
{
    if (!isVisible) return;
    if (!isActive) return;

    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);

    mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

bool Collider::Collision(Collider* collider)
{
    if (!isActive || !collider->isActive) return false;

    collider->UpdateWorld();
    UpdateWorld();

    switch (collider->type)
    {
    case Collider::BOX:
        return BoxCollision((BoxCollider*)collider);
        break;
    case Collider::SPHERE:
        return SphereCollision((SphereCollider*)collider);
        break;
    case Collider::CAPSULE:
        return CapsuleCollision((CapsuleCollider*)collider);
        break;
    }

    return false;
}
