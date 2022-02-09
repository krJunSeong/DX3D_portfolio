#pragma once

class Grid;

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Matrix projection;

    MatrixBuffer* projectionBuffer;
    MatrixBuffer* orthoBuffer;
    MatrixBuffer* uiViewBuffer;

    LightBuffer* lightBuffer;

    D3D11_VIEWPORT viewport;

    SamplerState* samplerState;

    Camera* camera;
    Frustum* frustum;

    BlendState* blendState[2];
    DepthStencilState* depthState[2];

    Environment();
    ~Environment();

public:
    void Set();
    void PostSet();

    void SetImGui();

    Camera* GetCamrea() { return camera; }
    Frustum* GetFrustum() { return frustum; }
    Matrix GetProjection() { return projection; }

    LightBuffer::Light* AddLight();
private:
    void SetViewport();
    void CreateProjection();

    void SetLight(UINT index);

    void Save();
    void Load();
};