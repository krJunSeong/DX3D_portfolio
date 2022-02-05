#pragma once

class Scene;

class SceneManager : public Singleton<SceneManager>
{
private:
    friend class Singleton;

    map<string, Scene*> scenes;

    vector<Scene*> curScenes;

    SceneManager();
    ~SceneManager();

public:
    void Update();

    void PreRender();
    void Render();
    void PostRender();
    void GUIRender();

    Scene* Create(string key, Scene* scene);

    Scene* Add(string key);
    void Remove(string key);
};
