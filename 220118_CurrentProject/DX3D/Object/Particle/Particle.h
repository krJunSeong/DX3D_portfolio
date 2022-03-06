#pragma once

class Particle
{
protected:
    const UINT MAX_COUNT = 1000;

    bool isActive;
    Vector3 position;

    Material* material;
    VertexBuffer* vertexBuffer;

    GeometryShader* geometryShader;

    BlendState* blendState[2];
    DepthStencilState* depthState[2];

    UINT particleCount = 0;

    Float2 size;
public:
    Particle();
    ~Particle();

    virtual void Update() {}
    virtual void Render();
    virtual void GUIRender() {}

    virtual void Play(Vector3 position);

    void Stop();

    Float2 GetSize(){return size;}
    void SetSize(Float2 input){ size = input;}
};