#pragma once

class Sphere : public Transform
{
private:
    typedef VertexUVNormalTangent VertexType;

    Mesh* mesh;
    Material* material;

    WorldBuffer* worldBuffer;

    vector<VertexType> vertices;
    vector<UINT> indices;

    UINT sliceCount;
    UINT stackCount;

    float radius;

public:
    Sphere(float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
    ~Sphere();

    void Render();
    void SetRender();

    Material* GetMaterial() { return material; }

    float GetRadius() { return radius; }

    UINT GetIndexCount() { return indices.size(); }
private:
    void CreateMesh();
    void CreateTangent();
};