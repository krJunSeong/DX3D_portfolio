#pragma once

class Quad : public Transform
{
private:
    typedef VertexUVNormalTangent VertexType;

    Material* material;
    Mesh* mesh;
    UINT width, height;

    vector<VertexType> vertices;
    vector<UINT> indices;

    WorldBuffer* worldBuffer;
    ColorBuffer* colorBuffer;

    Float2 size;
public:
    Quad(Float2 size = Float2(1, 1));
    ~Quad();

    void Render();

    Material* GetMaterial() { return material; }
    Float2 GetSizeScale() { return Float2(size.x * scale.x * 0.5, size.y * scale.y * 0.5); }

    void SetColor(Float4 color) { colorBuffer->data.color = color; }
private:
    void CreateMesh();
};