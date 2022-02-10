#pragma once

class ParticleScene : public Scene
{
private:
    Particle* particle;

    BoxCollider* collider;
public:
    ParticleScene();
    ~ParticleScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};