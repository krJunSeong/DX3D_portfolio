#pragma once

class ReportRobot_211201;

class TerrainEditor : public Transform
{
private:
    const float MIN_HEIGHT = 0.0f;
    const float MAX_HEIGHT = 10.0f;

    typedef VertexUVNormalTangentAlpha VertexType;

    Material* material;
    Mesh* mesh;

    vector<VertexType> vertices;
    vector<UINT> indices;

    UINT width, height;

    WorldBuffer* worldBuffer;

    vector<ReportRobot_211201*> robots;
    vector<Cube*> cubes;

    Texture* heightMap;
    Texture* secondMap;
    Texture* thirdMap;
    Texture* fourthMap;

    RasterizerState* rasterizerState;

    // 211214
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

    // Terrain Brush
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
    void AdjustHeight(); // adjust: 조절하다, 높이조절
    void AdjustAlpha();   // 

    void InitNormalTangent();
    void CreateMesh();
    void CreateNormal();
    void CreateTangent();

    // ------------------- Texture Out -----------------
    void SaveHeightMap();
    void LoadHeightMap();

    void SaveAlphaMap();
    void LoadAlphaMap();

    // ---------------------------- REPORT ----------------------------
    ReportRobot_211201* CreateRobot(Float3 pos);

    void CreateComputeData();
    // -----------------------------------------------------------------
};