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

    Camera* GetCamera() { return camera; }
    Frustum* GetFrustum() { return frustum; }
    LightBuffer* GetLightBuffer() { return lightBuffer;}
    Matrix GetProjection() { return projection; }

    LightBuffer::Light* AddLight();

    void Save(string name = "Light.info");
    void Load(string name = "Light.info");
private:
    void SetViewport();
    void CreateProjection();

    void SetLight(UINT index);

};