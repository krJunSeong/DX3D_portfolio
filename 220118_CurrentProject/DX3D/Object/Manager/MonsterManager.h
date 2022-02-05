#pragma once

class MonsterManager : public Singleton<MonsterManager>
{
private:
    friend class Singleton;

    vector<Chuchu*> chuchus;

    const float spawnLimitTime = 5.0f;
    float spawnDeltaTime = 0.0f;
    float playTime = 0.0f;

    Terrain* terrain;
    Quad* quad;
 
    MonsterManager();
    ~MonsterManager();

public:
    void Update();
    void Render();
    void PostRender();
    void GUIRender();
    void CreateChuchu(float poolCount = 5.0f);
    void ChuchuSpawn();
    void SpawnInit(Chuchu* chuchu);

    void SetTerrain(Terrain* terrain);
    void SetQuad(Quad* quad);

    Chuchu* RayCollision(IN Ray ray, OUT Vector3* hitPoint);
};