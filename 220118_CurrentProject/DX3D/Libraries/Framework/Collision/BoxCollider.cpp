#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size)
    : size(size)
{
    type = BOX;
    CreateMesh();
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
    UpdateWorld();

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    Obb box = GetObb();

    Vector3 min = box.halfSize * -1.0f;
    Vector3 max = box.halfSize;

    Vector3 delta = box.position - ray.position;

    ray.direction.Normalize();

    for (UINT i = 0; i < 3; i++)
    {
        Vector3 axis = box.axis[i];
        float e = Vector3::Dot(axis, delta);
        float f = Vector3::Dot(ray.direction, axis);

        if (abs(f) > FLT_EPSILON)
        {
            float t1 = (e + min[i]) / f;
            float t2 = (e + max[i]) / f;

            if (t1 > t2) swap(t1, t2);

            if (t2 < tMax) tMax = t2;
            if (t1 > tMin) tMin = t1;
            if (tMin > tMax) return false;
        }
        else
        {
            if (min[i] > e || max[i] < e) return false;
        }
    }

    if (contact != nullptr)
    {
        contact->distance = tMin;
        contact->hitPoint = ray.position + ray.direction * tMin;
    }

    return true;
}

bool BoxCollider::BoxCollision(BoxCollider* collider)
{
    Obb box1 = GetObb();
    Obb box2 = collider->GetObb();

    Vector3 D = box2.position - box1.position;

    for (UINT i = 0; i < 3; i++)
    {
        if (SeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (SeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[j]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            Vector3 cross = Vector3::Cross(box1.axis[i], box2.axis[j]);
            if (SeperateAxis(D, cross, box1, box2)) return false;
        }
    }

    return true;
}

bool BoxCollider::SphereCollision(SphereCollider* collider)
{
    Obb box = GetObb();

    Vector3 pos = box.position;//박스에서 구에 가장 가까운 점

    for (UINT i = 0; i < 3; i++)
    {
        float length = Vector3::Dot(box.axis[i], collider->GlobalPos() - box.position);

        float mult = (length < 0.0f) ? -1.0f : 1.0f;

        length = min(abs(length), box.halfSize[i]);

        pos += box.axis[i] * length * mult;
    }

    float distance = Distance(collider->GlobalPos(), pos);

    return distance <= collider->Radius();
}

bool BoxCollider::CapsuleCollision(CapsuleCollider* collider)
{
    return collider->BoxCollision(this);
}

BoxCollider::Obb BoxCollider::GetObb()
{
    obb.position = globalPosition;

    obb.axis[0] = Right();
    obb.axis[1] = Up();
    obb.axis[2] = Forward();

    obb.halfSize = size * 0.5f * globalScale;

    return obb;
}

void BoxCollider::CreateMesh()
{
    Vector3 halfSize = size * 0.5f;

    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z);
    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z);

    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z);
    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z);

    indices = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

    mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
        indices.data(), indices.size());
}

bool BoxCollider::SeperateAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2)
{
    float distance = abs(Vector3::Dot(D, axis));

    float a = 0.0f;
    float b = 0.0f;

    for (UINT i = 0; i < 3; i++)
    {
        a += abs(Vector3::Dot(box1.axis[i] * box1.halfSize[i], axis));
        b += abs(Vector3::Dot(box2.axis[i] * box2.halfSize[i], axis));
    }

    return distance > a + b;
}
