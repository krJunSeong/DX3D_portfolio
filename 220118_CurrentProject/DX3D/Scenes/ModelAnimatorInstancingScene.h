#pragma once

class ModelAnimatorInstancingScene : public Scene
{
private:
    ModelAnimatorInstancing* modelAnimatorInstancing;

    int instanceID;
    int selectClip;
public:
    ModelAnimatorInstancingScene();
    ~ModelAnimatorInstancingScene();
        
    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};