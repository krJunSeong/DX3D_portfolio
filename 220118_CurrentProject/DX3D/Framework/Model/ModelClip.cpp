#include "Framework.h"

ModelClip::ModelClip()
    : tickPerSecond(0), frameCount(0), playTime(0), duration(0), isRootMotion(false)
{
}

ModelClip::~ModelClip()
{
    for (pair<string, KeyFrame*> frame : keyFrames)
        delete frame.second;
}

void ModelClip::Init()
{
    eventIter = Events.begin();
    playTime = 0.0f;
    duration = frameCount / tickPerSecond;
}

void ModelClip::Excute()
{
    if (Events.empty()) return;
    if (eventIter == Events.end()) return;

    float radio = playTime / duration;

    if (eventIter->first > radio) return;

    eventIter->second();
    eventIter++;
}

void ModelClip::InstancingInit(UINT instance)
{
    instancingEventIter[instance] = InstancingEvents[instance].begin();
    duration = frameCount / tickPerSecond;
}

void ModelClip::InstancingExcute(UINT instance, float time)
{
    if (InstancingEvents[instance].empty()) return;
    if (instancingEventIter[instance] == InstancingEvents[instance].end()) return;

    float radio = time / duration;

    if (instancingEventIter[instance]->first > radio) return;

    instancingEventIter[instance]->second();
    instancingEventIter[instance]++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (keyFrames.count(boneName) == 0)
        return nullptr;

    return keyFrames[boneName];
}

void ModelClip::ResizeInstancing(UINT size)
{
    instancingEventIter.resize(size);
    InstancingEvents.resize(size);
}
