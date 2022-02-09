#pragma once

class ModelAnimatorInstancing : public ModelAnimator
{
private:
    vector<Transform*> transforms;
    InstanceData instanceDatas[MAX_INSTANCE];

    VertexBuffer* instanceBuffer;
    FrameInstancingBuffer* frameInstancingBuffer;

    UINT drawCount;
    Vector3 size;
public:
    ModelAnimatorInstancing(string name);
    ~ModelAnimatorInstancing();

    void Update();
    void Render();

    Transform* Add();

    void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 0.1f);

    Matrix GetTransformByNode(UINT instance, int nodeIndex);

    void AddEvent(UINT instance, int clip, float time, Event event);
private:
    void UpdateFrames();
    void UpdateFrame(FrameInstancingBuffer::Motion& motion, UINT instance);
    void UpdateTransforms();
};