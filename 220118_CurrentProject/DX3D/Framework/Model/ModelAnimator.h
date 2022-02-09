#pragma once

class ModelAnimator : public Transform
{
protected:
    string name;

    ModelReader* reader;

    vector<ModelClip*> clips;

    FrameBuffer* frameBuffer;
    WorldBuffer* worldBuffer;

    ClipTransform* clipTransforms;
    ClipTransform* nodeTransforms;
    ClipTransform* rootTransforms;

    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* srv;

    bool isPlay;

    int rootIndex;
public:
    ModelAnimator(string name);
    ~ModelAnimator();

    void Update();
    void Render();

    void ReadClip(string clipName, UINT clipNum = 0, bool isRootMotion = false);

    void PlayClip(UINT clip, float speed = 1.0f, float takeTime = 0.2f);
    void StopClip() { isPlay = false; }

    Matrix GetTransformByNode(int nodeIndex);

    ModelReader* GetReader() { return reader; }
protected:
    void UpdateFrame();

    void CreateTexture();
    void CreateClipTransform(UINT index);
};