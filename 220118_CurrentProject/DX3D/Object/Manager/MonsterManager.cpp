#include "framework.h"

MonsterManager::MonsterManager()
{
}

MonsterManager::~MonsterManager()
{
    for (Chuchu* chuchu : chuchus)
        delete chuchu;
}

void MonsterManager::Update()
{
    for (Chuchu* chuchu : chuchus)
        chuchu->Update();

    ChuchuSpawn();
}

void MonsterManager::Render()
{
    for (Chuchu* chuchu : chuchus)
    {
        chuchu->Render();
    }
}

void MonsterManager::PostRender()
{
    for (Chuchu* chuchu : chuchus)
        chuchu->PostRender();
}

void MonsterManager::GUIRender()
{
    for (Chuchu* chuchu : chuchus)
        chuchu->GUIRender();
}

void MonsterManager::SetTerrain(Terrain* terrain)
{
    for (Chuchu* chuchu : chuchus)
        chuchu->SetTerrain(terrain);
}

void MonsterManager::SetQuad(Quad* quad)
{
    for(Chuchu* chuchu : chuchus)
        chuchu->SetQuad(quad);
}

Chuchu* MonsterManager::RayCollision(IN Ray ray, OUT Vector3* hitPoint)
{
    return nullptr;
}

void MonsterManager::CreateChuchu(float poolCount)
{
    for(int i = 0; i < poolCount; i++)
    {
        Chuchu* chuchu = new Chuchu();
        chuchu->isActive = false;

        chuchus.push_back(chuchu);
    }
}

void MonsterManager::ChuchuSpawn()
{
    spawnDeltaTime += DELTA;
    
    if(spawnDeltaTime >= spawnLimitTime)
    {
        spawnDeltaTime = 0;

        for(Chuchu* chuchu : chuchus)
        {
            if(!chuchu->isActive) 
            {
                SpawnInit(chuchu);        
                break;
            }
        }
    }
}

void MonsterManager::SpawnInit(Chuchu* chuchu)
{
    Float2 pos = chuchu->GetQuad()->GetSizeScale();
    chuchu->Init();

    chuchu->position.x = Random(-pos.x, +pos.x);
    chuchu->position.z = Random(-pos.y, +pos.y);

    chuchu->UpdateWorld();
}