#pragma once

class Terrain : public Transform
{
private:
    const float MAX_HEIGHT = 10.0f;

    typedef VertexUVNormal VertexType;

    Material* material;
    Mesh* mesh;

    vector<VertexType> vertices;
    vector<UINT> indices;

    UINT width, height;

    WorldBuffer* worldBuffer;

    Texture* heightMap;
    Texture* alphaMap;

    Texture* secondMap;
    Texture* thirdMap;

    RasterizerState* rasterizerState;

    struct InputDesc
    {
        UINT index;
        Float3 v0, v1, v2;
    };
    struct OutputDesc
    {
        int picked;
        float distance;
    };

    ComputeShader* computeShader;
    RayBuffer* rayBuffer;
    StructuredBuffer* structuredBuffer;
    InputDesc* input;
    OutputDesc* output;

    UINT size;
public:
	Terrain(wstring alphaMapName = L"TestAlpha.png",
		wstring heightMapName = L"HeightMap.png");
	~Terrain();

    void Render();

    float GetHeight(Vector3 position = {0,0,0});

    Vector3 Picking();

    Float2 GetSize() { return Float2(width, height); }
private:
    void CreateMesh();
    void CreateNormal();
    void CreateCompute();
};