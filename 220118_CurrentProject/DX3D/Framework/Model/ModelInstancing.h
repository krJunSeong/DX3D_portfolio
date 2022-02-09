#pragma once

class ModelInstancing
{
protected:
    ModelReader* reader;

    vector<Transform*> transforms;
    
    InstanceData instanceDatas[MAX_INSTANCE];

    VertexBuffer* instanceBuffer;

    UINT drawCount;

    Vector3 size;
public:
    ModelInstancing(string name);
    ~ModelInstancing();

    void Update();
    void Render();
    void GUIRender();

    Transform* Add();
};