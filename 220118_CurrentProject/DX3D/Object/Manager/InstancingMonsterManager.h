#pragma once

class InstancingMonsterManager : public Singleton<InstancingMonsterManager>
{
private:
    friend class Singleton;

    ModelAnimatorInstancing* monsterInstancing;
    ModelInstancing* swordInstancing;

    vector<InstancingChuchu*> chuchus;

    float spawnDelayTime = 0.0f;
    float playTime = 0.0f;

    Terrain* land;
    Quad* quad;
    Jean* player;

    InstancingMonsterManager();
    ~InstancingMonsterManager();

public:
    void Update();
    void Render();
    void PostRender();
    void GUIRender();

    void CreateMonsters(UINT poolCount = 20);
    void Collision(Collider* collider, float damage);

    void SetTerrain(Terrain* land);
    void SetQuad(Quad* quad);
    void SetPlayer(Jean* player);

    InstancingChuchu* RayCollision(IN Ray ray, OUT Vector3* hitPoint);
    const vector<InstancingChuchu*>& GetChuchus(){return chuchus;}
private:
    void Spawn();
};