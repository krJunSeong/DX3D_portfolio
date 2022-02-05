#include "Framework.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
    for (pair<string, Scene*> scene : scenes)
        delete scene.second;
}

void SceneManager::Update()
{
    for (Scene* scene : curScenes)
        scene->Update();
}

void SceneManager::PreRender()
{
    for (Scene* scene : curScenes)
        scene->PreRender();
}

void SceneManager::Render()
{
    for (Scene* scene : curScenes)
        scene->Render();
}

void SceneManager::PostRender()
{
    for (Scene* scene : curScenes)
        scene->PostRender();
}

void SceneManager::GUIRender()
{
    for (Scene* scene : curScenes)
        scene->GUIRender();
}

Scene* SceneManager::Create(string key, Scene* scene)
{
    if (scenes.count(key) > 0)
        return scenes[key];

    scenes[key] = scene;

    return scene;
}

Scene* SceneManager::Add(string key)
{
    if (scenes.count(key) == 0) return nullptr;

    for (Scene* scene : curScenes)
    {
        if (scenes[key] == scene)
            return nullptr;
    }

    curScenes.push_back(scenes[key]);
    curScenes.back()->Start();

    return scenes[key];
}

void SceneManager::Remove(string key)
{
    /*
    vector<Scene*>::iterator iter = curScenes.begin();

    while(iter != curScenes.end())
    {
        if((*iter) == scenes[key])
        {
            iter = curScenes.erase(iter);
        }
        else
        {
            iter++;
        }
    }*/

    for (UINT i = 0; i < curScenes.size(); i++)
    {
        if (curScenes[i] == scenes[key])
        {
            scenes[key]->End();
            curScenes.erase(curScenes.begin() + i);
            return;
        }
    }
}
