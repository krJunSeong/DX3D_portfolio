#include "Framework.h"

int GameMath::Random(int min, int max)
{
    return rand() % (max - min) + min;
}

float GameMath::Random(float min, float max)
{
    float normal = rand() / (float)RAND_MAX;

    return min + (max - min) * normal;
}


Vector3 GameMath::Random(Vector3 v1, Vector3 v2)
{
    Vector3 result;
    result.x = Random(v1.x, v2.x);
    result.y = Random(v1.y, v2.y);
    result.z = Random(v1.x, v2.x);

    return result;
}

float GameMath::Distance(const Vector3& v1, const Vector3& v2)
{
    return (v1 - v2).Length();
}

float GameMath::Saturate(const float& value)
{
    return max(0.0f, min(1.0f, value));
}

Vector3 GameMath::ClosestPointOnLine(const Vector3& start, const Vector3& end, const Vector3& point)
{
    Vector3 line = end - start;

    float x = Vector3::Dot(line, point - start);
    float y = Vector3::Dot(line, line);

    float t = Saturate(x / y);

    return start + line * t;
}

Vector3 GameMath::RadianToQuaternion(Vector3 q)
{
    Vector3 angles;

    float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    angles.x = atan2(sinr_cosp, cosr_cosp);

    float sinp = 2 * (q.w * q.y - q.z * q.x);
    if (abs(sinp) >= 1)
        angles.y = copysign(XM_PI / 2, sinp);
    else
        angles.y = asin(sinp);

    float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    angles.z = atan2(siny_cosp, cosy_cosp);

    return angles;
}
