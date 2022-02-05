#pragma once

class ModelMesh
{
private:
    string name;

    Material* material;
    Mesh* mesh;

    UINT vertexCount;
    UINT indexCount;

    ModelVertex* vertices;
    UINT* indices;
public:
    ModelMesh(string name);
    ~ModelMesh();

    void Render();
    void Render(UINT instanceCount);

    void CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount);

    void SetMaterial(Material* material) { this->material = material; }

    void SetBox(Vector3* minBox, Vector3* maxBox);
};