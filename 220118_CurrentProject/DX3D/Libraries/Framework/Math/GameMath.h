#pragma once

namespace GameMath
{
    int Random(int min, int max);
    float Random(float min, float max);

    float Distance(const Vector3& v1, const Vector3& v2);

    float Saturate(const float& value);

    Vector3 ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point);

    Vector3 RadianToQuaternion(Vector3 q);
}