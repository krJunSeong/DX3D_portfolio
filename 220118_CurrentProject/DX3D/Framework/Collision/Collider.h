#pragma once

struct Ray
{
    Vector3 position;
    Vector3 direction;
};

struct Contact
{
    string tag;

    Vector3 hitPoint;
    float distance;
};

class BoxCollider;
class SphereCollider;
class CapsuleCollider;

class Collider : public Transform
{
public:
    enum Type
    {
        BOX, SPHERE, CAPSULE, NONE
    };

    static bool isVisible;

protected:
    Material* material;
    Mesh* mesh;

    MatrixBuffer* worldBuffer;

    Type type;

    vector<Vertex> vertices;
    vector<UINT> indices;

public:
    Collider();
    virtual ~Collider();

    void Render();

    bool Collision(Collider* collider);

    virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;
    virtual bool BoxCollision(BoxCollider* collider) = 0;
    virtual bool SphereCollision(SphereCollider* collider) = 0;
    virtual bool CapsuleCollision(CapsuleCollider* collider) = 0;

    void SetColor(Float4 color) { material->GetData().diffuse = color; }
    void SetColor(float r, float g, float b) { material->GetData().diffuse = Float4(r, g, b, 1); }
protected:
    virtual void CreateMesh() = 0;
};