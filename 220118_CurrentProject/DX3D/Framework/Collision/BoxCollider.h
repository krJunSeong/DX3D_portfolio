#pragma once

class BoxCollider : public Collider
{
public:
    struct Obb
    {
        Vector3 position;

        Vector3 axis[3];
        Vector3 halfSize;
    };

private:
    Vector3 size;

    Obb obb;
public:
    BoxCollider(Vector3 size = Vector3(1, 1, 1));
    ~BoxCollider();

    virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
    virtual bool BoxCollision(BoxCollider* collider) override;
    virtual bool SphereCollision(SphereCollider* collider) override;
    virtual bool CapsuleCollision(CapsuleCollider* collider) override;

    Obb GetObb();
private:
    virtual void CreateMesh() override;

    bool SeperateAxis(Vector3 D, Vector3 axis, Obb box1, Obb box2);
};
