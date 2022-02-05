#pragma once

class DefenceScene : public Scene
{
private:
    Sky* sky;
    Terrain* terrain;

    Amber* amber;

    SphereCollider* startPos;
public:
    DefenceScene();
    ~DefenceScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

    virtual void Start();
    virtual void End();
};