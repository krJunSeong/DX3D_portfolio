#pragma once

class SphereCollider : public Collider
{
private:
    float radius;

    UINT stackCount;
    UINT sliceCount;

public:
    SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
    ~SphereCollider();

    virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;

    virtual bool BoxCollision(BoxCollider* collider) override;
    virtual bool SphereCollision(SphereCollider* collider) override;
    virtual bool CapsuleCollision(CapsuleCollider* collider) override;

    float Radius() { return radius * max(globalScale.x, max(globalScale.y, globalScale.z)); }

private:
    virtual void CreateMesh() override;
};