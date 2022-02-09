#include "Framework.h"

ModelAnimator::ModelAnimator(string name)
    : name(name), texture(nullptr), isPlay(false)
{
    tag = name;

    reader = new ModelReader(name);
    reader->SetShader(L"Lighting.hlsl");

    frameBuffer = new FrameBuffer();
    worldBuffer = new WorldBuffer();
    worldBuffer->SetType(ObjectType::ANIM);
}

ModelAnimator::~ModelAnimator()
{
    delete reader;

    for (ModelClip* clip : clips)
        delete clip;

    delete frameBuffer;
    delete worldBuffer;

    delete[] clipTransforms;
    delete[] nodeTransforms;

    texture->Release();
    srv->Release();
}

void ModelAnimator::Update()
{
    UpdateFrame();
    UpdateWorld();
}

void ModelAnimator::Render()
{
    if (texture == nullptr)
        CreateTexture();

    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);
    frameBuffer->SetVSBuffer(3);

    DC->VSSetShaderResources(0, 1, &srv);

    reader->Render();
}

void ModelAnimator::ReadClip(string clipName, UINT clipNum, bool isRootMotion)
{
    string path = "ModelData/Clips/" + name + "/" + clipName + to_string(clipNum) + ".clip";

    BinaryReader r(path);

    ModelClip* clip = new ModelClip();
    clip->name = r.String();
    clip->frameCount = r.UInt();
    clip->tickPerSecond = r.Float();
    clip->isRootMotion = isRootMotion;

    UINT boneCount = r.UInt();
    for (UINT i = 0; i < boneCount; i++)
    {
        KeyFrame* keyFrame = new KeyFrame();
        keyFrame->boneName = r.String();

        UINT size = r.UInt();
        if (size > 0)
        {
            keyFrame->transforms.resize(size);

            void* ptr = (void*)keyFrame->transforms.data();
            r.Byte(&ptr, sizeof(KeyTransform) * size);
        }
        clip->keyFrames[keyFrame->boneName] = keyFrame;
    }

    clips.push_back(clip);
}

void ModelAnimator::PlayClip(UINT clip, float speed, float takeTime)
{
    isPlay = true;

    frameBuffer->data.next.clip = clip;
    frameBuffer->data.next.speed = speed;
    frameBuffer->data.takeTime = takeTime;

    clips[clip]->Init();
}

Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
    if (texture == nullptr) return XMMatrixIdentity();

    FrameBuffer::Frame& curFrame = frameBuffer->data.cur;

    Matrix cur = nodeTransforms[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
    Matrix next = nodeTransforms[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];

    Matrix curAnim = LERP(cur, next, curFrame.time);

    FrameBuffer::Frame& nextFrame = frameBuffer->data.next;

    if (nextFrame.clip == -1)
        return curAnim;

    cur = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
    next = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

    Matrix nextAnim = LERP(cur, next, nextFrame.time);

    return LERP(curAnim, nextAnim, frameBuffer->data.tweenTime);
}

void ModelAnimator::UpdateFrame()
{
    if (!isPlay) return;

    FrameBuffer::Data& frameData = frameBuffer->data;

    {//CurAnim
        ModelClip* clip = clips[frameData.cur.clip];

        clip->playTime += frameData.cur.speed * DELTA;

        frameData.cur.time += clip->tickPerSecond * frameData.cur.speed * DELTA;

        if (frameData.cur.time >= 1.0f)
        {
            frameData.cur.curFrame = (frameData.cur.curFrame + 1) % (clip->frameCount - 1);
            frameData.cur.time = 0.0f;
        }

        clip->Excute();
    }

    {//NextAnim
        if (frameData.next.clip < 0) return;

        ModelClip* clip = clips[frameData.next.clip];

        frameData.tweenTime += DELTA / frameData.takeTime;

        if (frameData.tweenTime >= 1.0f)//End Blending
        {
            frameData.cur = frameData.next;
            frameData.tweenTime = 0.0f;

            frameData.next.clip = -1;
            frameData.next.curFrame = 0;
            frameData.next.time = 0.0f;
        }
        else
        {
            frameData.next.time += clip->tickPerSecond * frameData.next.speed * DELTA;

            if (frameData.next.time >= 1.0f)
            {
                frameData.next.curFrame = (frameData.next.curFrame + 1) % (clip->frameCount - 1);
                frameData.next.time = 0.0f;
            }
        }
    }
}

void ModelAnimator::CreateTexture()
{
    UINT clipCount = clips.size();

    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];
    rootTransforms = new ClipTransform[clipCount];

    for (UINT i = 0; i < clipCount; i++)
        CreateClipTransform(i);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = MAX_BONE * 4;
    desc.Height = MAX_FRAME;
    desc.ArraySize = clipCount;
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.MipLevels = 1;
    desc.SampleDesc.Count = 1;

    UINT pageSize = MAX_BONE * sizeof(Matrix) * MAX_FRAME;

    void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

    for (UINT c = 0; c < clipCount; c++)
    {
        UINT start = c * pageSize;

        for (UINT y = 0; y < MAX_FRAME; y++)
        {
            void* temp = (BYTE*)p + MAX_BONE * y * sizeof(Matrix) + start;

            VirtualAlloc(temp, MAX_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[c].transform[y], MAX_BONE * sizeof(Matrix));
        }
    }

    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    for (UINT c = 0; c < clipCount; c++)
    {
        void* temp = (BYTE*)p + c * pageSize;

        subResource[c].pSysMem = temp;
        subResource[c].SysMemPitch = MAX_BONE * sizeof(Matrix);
        subResource[c].SysMemSlicePitch = pageSize;
    }

    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    delete[] subResource;
    VirtualFree(p, 0, MEM_RELEASE);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.ArraySize = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
    ModelClip* clip = clips[index];

    for (UINT f = 0; f < clip->frameCount; f++)
    {
        UINT nodeIndex = 0;
        for (NodeData node : reader->GetNodes())
        {
            Matrix animation;
            KeyFrame* frame = clip->GetKeyFrame(node.name);
            if (frame != nullptr)
            {
                KeyTransform& transform = frame->transforms[f];

                animation = XMMatrixTransformation(XMVectorZero(), XMQuaternionIdentity(),
                    Vector3(transform.scale), XMVectorZero(),
                    XMLoadFloat4(&transform.rotation),
                    Vector3(transform.position));
            }
            else
            {
                animation = XMMatrixIdentity();
            }

            Matrix parent;
            int parentIndex = node.parent;

            if (parentIndex < 0)
                parent = XMMatrixIdentity();
            else
            {
                if (clip->isRootMotion)
                    parent = rootTransforms[index].transform[f][parentIndex];
                else
                    parent = nodeTransforms[index].transform[f][parentIndex];
            }

            nodeTransforms[index].transform[f][nodeIndex] = animation * parent;
            rootTransforms[index].transform[f][nodeIndex] = animation * parent;

            if (node.index == rootIndex)
                rootTransforms[index].transform[f][nodeIndex] = node.transform * parent;

            if (reader->GetBone(node.name) >= 0)
            {
                int boneIndex = reader->GetBone(node.name);

                Matrix transform = reader->GetBones()[boneIndex].offset;
                if (clip->isRootMotion)
                    transform *= rootTransforms[index].transform[f][nodeIndex];
                else
                    transform *= nodeTransforms[index].transform[f][nodeIndex];

                clipTransforms[index].transform[f][boneIndex] = transform;
            }

            nodeIndex++;
        }
    }
}
