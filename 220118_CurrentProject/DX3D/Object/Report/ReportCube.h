#pragma once

class ReportCube
{
private:
    VertexShader* vertexShader;
    PixelShader* pixelShader;

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;

    vector<VertexUV> vertices;
    vector<UINT> indices;

    MatrixBuffer* worldBuffer;
    Texture* texture;

public:
    ReportCube(Float3 pos);
    ReportCube(Float3 pos, float r, Float3 color);
    ~ReportCube();

    void Update();
    void Render();

};