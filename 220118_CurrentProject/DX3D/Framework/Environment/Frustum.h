#pragma once

class Frustum
{
private:
    Vector3 planes[6];

    Matrix projection;
    Matrix view;

    float a, b, c, d;
public:
    Frustum(Matrix projection);
    ~Frustum();

    void Update();

    bool ContainPoint(Vector3 point);
    bool ContainSphere(Vector3 center, float radius);
    bool ContainBox(Vector3 center, Vector3 size);
};