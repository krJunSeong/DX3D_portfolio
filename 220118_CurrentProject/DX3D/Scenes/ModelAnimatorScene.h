#pragma once

class ModelAnimatorScene : public Scene
{
private:
    ModelAnimator* modelAnimator;

public:
    ModelAnimatorScene();
    ~ModelAnimatorScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};
