#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
    : radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
    type = SPHERE;
    CreateMesh();
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    UpdateWorld();

    Vector3 P = ray.position;
    Vector3 D = ray.direction;

    Vector3 C = globalPosition;
    Vector3 A = P - C;

    float b = Vector3::Dot(D, A);
    float c = Vector3::Dot(A, A) - Radius() * Radius();

    if (b * b >= c)
    {
        if (contact != nullptr)
        {
            float t = -b - sqrt(b * b - c);

            contact->distance = t;
            contact->hitPoint = P + D * t;
        }

        return true;
    }

    return false;
}

bool SphereCollider::BoxCollision(BoxCollider* collider)
{
    return collider->SphereCollision(this);
}

bool SphereCollider::SphereCollision(SphereCollider* collider)
{
    float distance = Distance(globalPosition, collider->globalPosition);

    return distance <= Radius() + collider->Radius();
}

bool SphereCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return collider->SphereCollision(this);
}

void SphereCollider::CreateMesh()
{
    float phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            Vertex vertex;

            vertex.position.x = sin(phi) * cos(theta) * radius;
            vertex.position.y = cos(phi) * radius;
            vertex.position.z = sin(phi) * sin(theta) * radius;

            vertices.push_back(vertex);
        }
    }

    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            indices.push_back((sliceCount + 1) * i + j);//0            
            indices.push_back((sliceCount + 1) * i + j + 1);//1            

            indices.push_back((sliceCount + 1) * i + j);//0    
            indices.push_back((sliceCount + 1) * (i + 1) + j);//2            
        }
    }

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}
