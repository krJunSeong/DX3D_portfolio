#include "Framework.h"

Frustum::Frustum(Matrix projection)
    :projection(projection)
{
}

Frustum::~Frustum()
{
}

void Frustum::Update()
{
    view = CAM->GetView();

    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);

    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);

    for (UINT i = 0; i < 6; i++)
        planes[i] = XMPlaneNormalize(planes[i]);
}

bool Frustum::ContainPoint(Vector3 point)
{
    for (UINT i = 0; i < 6; i++)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);

        if (dot.x < 0.0f)
            return false;
    }

    return true;
}

bool Frustum::ContainSphere(Vector3 center, float radius)
{
    Vector3 edge;
    Vector3 dot;

    for (UINT i = 0; i < 6; i++)
    {
        //1
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //2
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //3
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //4
        edge.x = center.x - radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //5
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z - radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //6
        edge.x = center.x + radius;
        edge.y = center.y - radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //7
        edge.x = center.x - radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //8
        edge.x = center.x + radius;
        edge.y = center.y + radius;
        edge.z = center.z + radius;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        return false;
    }

    return true;
}

bool Frustum::ContainBox(Vector3 center, Vector3 size)
{
    Vector3 edge;
    Vector3 dot;
    Vector3 halfSize = size * 0.5f;

    for (UINT i = 0; i < 6; i++)
    {
        //1
        edge.x = center.x - halfSize.x;
        edge.y = center.y - halfSize.y;
        edge.z = center.z - halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //2
        edge.x = center.x + halfSize.x;
        edge.y = center.y - halfSize.y;
        edge.z = center.z - halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //3
        edge.x = center.x - halfSize.x;
        edge.y = center.y + halfSize.y;
        edge.z = center.z - halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //4
        edge.x = center.x - halfSize.x;
        edge.y = center.y - halfSize.y;
        edge.z = center.z + halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //5
        edge.x = center.x + halfSize.x;
        edge.y = center.y + halfSize.y;
        edge.z = center.z - halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //6
        edge.x = center.x + halfSize.x;
        edge.y = center.y - halfSize.y;
        edge.z = center.z + halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //7
        edge.x = center.x - halfSize.x;
        edge.y = center.y + halfSize.y;
        edge.z = center.z + halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        //8
        edge.x = center.x + halfSize.x;
        edge.y = center.y + halfSize.y;
        edge.z = center.z + halfSize.z;
        dot = XMPlaneDotCoord(planes[i], edge);
        if (dot.x > 0.0f)
            continue;

        return false;
    }

    return true;
}
