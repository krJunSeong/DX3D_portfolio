#pragma once

class InstancingScene : public Scene
{
private:
    struct InstanceData
    {
        Matrix transform;
        UINT index;
    };

    Sphere* sphere;

    vector<InstanceData> instanceDatas;
    VertexBuffer* instanceBuffer;

public:
    InstancingScene();
    ~InstancingScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};