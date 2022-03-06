#pragma once

class TerrainEditor : public Transform
{
private:
    const float MIN_HEIGHT = 0.0f;
    const float MAX_HEIGHT = 100.0f;

    typedef VertexUVNormalTangentAlpha VertexType;

    Material* material;
    Mesh* mesh;

    vector<VertexType> vertices;
    vector<UINT> indices;

    UINT width, height;

    WorldBuffer* worldBuffer;

    Texture* heightMap;

    Texture* secondMap;
    Texture* thirdMap;

    RasterizerState* rasterizerState;

    string selectHeightMap;
    string selectAlphaMap;

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

    BrushBuffer* brushBuffer;

    int editType;

    float adjustValue;
    UINT selectMap;
public:
    TerrainEditor(UINT width = 100, UINT height = 100);
    ~TerrainEditor();

    void Update();
    void Render();
    void GUIRender();

    Vector3 Picking();
    Vector3 ComputePicking();
private:
    void AdjustHeight();
    void AdjustAlpha();

    void InitNormalTangent();
    void CreateMesh();
    void CreateNormal();
    void CreateTangent();

    void CreateComputeData();

    void SaveHeightMap();
    void LoadHeightMap();

    void SaveAlphaMap();
    void LoadAlphaMap();
};