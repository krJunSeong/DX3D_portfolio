#include "framework.h"

InstancingMonsterManager::InstancingMonsterManager()
{
    string name = "Chuchu";
    monsterInstancing = new ModelAnimatorInstancing(name);
    monsterInstancing->ReadClip("Idle");
    monsterInstancing->ReadClip("Run");
    monsterInstancing->ReadClip("Attack");
    monsterInstancing->ReadClip("Hit");
    monsterInstancing->ReadClip("Death");
}

InstancingMonsterManager::~InstancingMonsterManager()
{
    delete monsterInstancing;

    for (InstancingChuchu* chuchu : chuchus)
        delete chuchu;
}

void InstancingMonsterManager::Update()
{
    monsterInstancing->Update();

    for (InstancingChuchu* chuchu : chuchus)
        chuchu->Update();

    Spawn();
}

void InstancingMonsterManager::Render()
{
    monsterInstancing->Render();

    for (InstancingChuchu* chuchu : chuchus)
        chuchu->Render();
}

void InstancingMonsterManager::PostRender()
{
    for(InstancingChuchu* chuchu : chuchus)
        chuchu->PostRender();
}

void InstancingMonsterManager::GUIRender()
{
    for (InstancingChuchu* chuchu : chuchus)
        chuchu->GUIRender();
}

void InstancingMonsterManager::CreateMonsters(UINT poolCount)
{
    chuchus.resize(poolCount);

    UINT count = 0;
    for (InstancingChuchu*& chuchu : chuchus)
    {
        chuchu = new InstancingChuchu(count++);
        chuchu->SetInstancing(monsterInstancing);
        Transform* transform = monsterInstancing->Add();
        transform->tag = "Chuchu";
        transform->Load();
        transform->isActive = false;
        chuchu->SetTransform(transform);
        chuchu->GetCollider()->SetParent(transform);

        chuchu->SetEvents();
    }
}

void InstancingMonsterManager::Collision(Collider* collider, float damage)
{
    for (InstancingChuchu* chuchu : chuchus)
    {
        if(chuchu->GetCollider()->Collision(collider))
            chuchu->Damaged(damage);
    }
}

void InstancingMonsterManager::SetTerrain(Terrain* land)
{
    this->land = land;

    for (InstancingChuchu* chuchu : chuchus)
    {
        chuchu->SetTerrain(land);
    }
}

void InstancingMonsterManager::SetQuad(Quad* quad)
{
    this->quad = quad;

    for (InstancingChuchu* chuchu : chuchus)
    {
        chuchu->SetQuad(quad);
    }
}

void InstancingMonsterManager::SetPlayer(Jean* player)
{
    for(InstancingChuchu* chuchu : chuchus)
        chuchu->SetTarget(player);
}

InstancingChuchu* InstancingMonsterManager::RayCollision(IN Ray ray, OUT Vector3* hitPoint)
{
    Contact contact;
    for (InstancingChuchu* chuchu : chuchus)
    {
       if (chuchu->GetCollider()->RayCollision(ray, &contact))
       {
           *hitPoint = contact.hitPoint;
           return chuchu;
       }
    }

    return nullptr;
}

void InstancingMonsterManager::Spawn()
{
    playTime += DELTA;

    if (playTime < spawnDelayTime) return;

    Vector3 pos;
    //pos.x = Random(-land->.x, land->GetSizeScale().x);
    //pos.z = Random(-land->.y, land->GetSizeScale().y);
    pos.y = quad->position.y;

    for (InstancingChuchu* chuchu : chuchus)
    {
        if (!chuchu->GetTransform()->isActive)
        {
            chuchu->SpawnInit();
            chuchu->GetTransform()->position = pos;

            playTime = 0.0f;
            spawnDelayTime = Random(0.5f, 1.5f);
            return;
        }
    }
}
