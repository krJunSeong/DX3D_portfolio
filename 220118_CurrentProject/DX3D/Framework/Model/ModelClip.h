#pragma once

class ModelClip
{
private:
    friend class ModelAnimator;
    friend class ModelAnimatorInstancing;

    string name;

    UINT frameCount;
    float tickPerSecond;
    float duration;

    float playTime;

    bool isRootMotion;

    unordered_map<string, KeyFrame*> keyFrames;

    map<float, Event>::iterator eventIter;
    vector< map<float, Event>::iterator> instancingEventIter;

    ModelClip();
    ~ModelClip();

    void Init();
    void Excute();

    void InstancingInit(UINT instance);
    void InstancingExcute(UINT instance, float time);

    KeyFrame* GetKeyFrame(string boneName);

public:
    map<float, Event> Events;
    vector<map<float, Event>> InstancingEvents;

    void ResizeInstancing(UINT size);

    bool IsRootMotion() { return isRootMotion; }
};