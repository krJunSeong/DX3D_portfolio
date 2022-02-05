#pragma once

class LightScene : public Scene
{
private:
    Quad* quad;

    Model* bunny;
    ModelAnimator* character;
    Sphere* sphere;

public:
    LightScene();
    ~LightScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};