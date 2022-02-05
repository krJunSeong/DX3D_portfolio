#pragma once

class ModelInstancingScene : public Scene
{
private:
    //vector<Model*> models;
    ModelInstancing* modelInstancing;

public:
    ModelInstancingScene();
    ~ModelInstancingScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;
};