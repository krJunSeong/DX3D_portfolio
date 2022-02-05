#include "Framework.h"

ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
    : ModelAnimator(name)
{
    reader->SetShader(L"AnimatorInstancing.hlsl");

    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
    frameInstancingBuffer = new FrameInstancingBuffer();

    size = reader->GetSize();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
    delete instanceBuffer;
    delete frameInstancingBuffer;
}

void ModelAnimatorInstancing::Update()
{
    UpdateFrames();
    UpdateTransforms();
}

void ModelAnimatorInstancing::Render()
{
    if (texture == nullptr)
        CreateTexture();

    frameInstancingBuffer->SetVSBuffer(4);
    DC->VSSetShaderResources(0, 1, &srv);

    instanceBuffer->IASet(1);

    reader->Render(drawCount);
}

Transform* ModelAnimatorInstancing::Add()
{
    Transform* transform = new Transform();
    transforms.push_back(transform);

    for (ModelClip* clip : clips)
    {
        clip->ResizeInstancing(transforms.size());
    }

    return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
    frameInstancingBuffer->data.motions[instance].next.clip = clip;
    frameInstancingBuffer->data.motions[instance].next.speed = speed;
    frameInstancingBuffer->data.motions[instance].takeTime = takeTime;
    frameInstancingBuffer->data.motions[instance].runningTime = 0.0f;

    clips[clip]->InstancingInit(instance);
}

Matrix ModelAnimatorInstancing::GetTransformByNode(UINT instance, int nodeIndex)
{
    if (texture == nullptr) return XMMatrixIdentity();

    FrameInstancingBuffer::Frame& curFrame = frameInstancingBuffer->data.motions[instance].cur;

    Matrix cur = nodeTransforms[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
    Matrix next = nodeTransforms[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];

    Matrix curAnim = LERP(cur, next, curFrame.time);

    FrameInstancingBuffer::Frame& nextFrame = frameInstancingBuffer->data.motions[instance].next;

    if (nextFrame.clip == -1)
        return curAnim;

    cur = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
    next = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

    Matrix nextAnim = LERP(cur, next, nextFrame.time);

    return LERP(curAnim, nextAnim, frameInstancingBuffer->data.motions[instance].tweenTime);
}

void ModelAnimatorInstancing::AddEvent(UINT instance, int clip, float time, Event event)
{
    clips[clip]->InstancingEvents[instance][time] = event;
}

void ModelAnimatorInstancing::UpdateFrames()
{
    for (UINT i = 0; i < transforms.size(); i++)
    {
        UpdateFrame(frameInstancingBuffer->data.motions[i], i);
    }

}

void ModelAnimatorInstancing::UpdateFrame(FrameInstancingBuffer::Motion& motion, UINT instance)
{
    {//CurAnim
        ModelClip* clip = clips[motion.cur.clip];

        motion.runningTime += motion.cur.speed * DELTA;

        motion.cur.time += clip->tickPerSecond * motion.cur.speed * DELTA;

        if (motion.cur.time >= 1.0f)
        {
            motion.cur.curFrame = (motion.cur.curFrame + 1) % (clip->frameCount - 1);
            motion.cur.time = 0.0f;
        }

        clip->InstancingExcute(instance, motion.runningTime);
    }

    {//NextAnim
        if (motion.next.clip < 0) return;

        ModelClip* clip = clips[motion.next.clip];

        motion.tweenTime += DELTA / motion.takeTime;

        if (motion.tweenTime >= 1.0f)//End Blending
        {
            motion.cur = motion.next;
            motion.tweenTime = 0.0f;

            motion.next.clip = -1;
            motion.next.curFrame = 0;
            motion.next.time = 0.0f;
        }
        else
        {
            motion.next.time += clip->tickPerSecond * motion.next.speed * DELTA;

            if (motion.next.time >= 1.0f)
            {
                motion.next.curFrame = (motion.next.curFrame + 1) % (clip->frameCount - 1);
                motion.next.time = 0.0f;
            }
        }
    }
}

void ModelAnimatorInstancing::UpdateTransforms()
{
    drawCount = 0;

    for (UINT i = 0; i < transforms.size(); i++)
    {
        if (!transforms[i]->isActive) continue;

        transforms[i]->UpdateWorld();
        transforms[i]->isFrustum = false;

        if (!FRUSTUM->ContainBox(transforms[i]->GlobalPos(), size * transforms[i]->GlobalScale())) continue;

        transforms[i]->isFrustum = true;
        instanceDatas[drawCount].transform = XMMatrixTranspose(transforms[i]->GetWorld());
        instanceDatas[drawCount].index = i;
        drawCount++;
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}
