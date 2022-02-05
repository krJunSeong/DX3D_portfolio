#include "framework.h"
#include "ReportScene.h"

ReportScene::ReportScene()
{
	land = new Quad();
	land->tag = "land";
	land->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Grace.png");
	land->Load();

	jean = new Jean();

	boss = new RockBoss();

	sky = new Sky(L"Textures/Landscape/Space.dds");
	//chuchu = new Chuchu();

	//house = new JapanHouse();
	//house->tag = "house";
	//house->Load();

	//gumdramon = new Gumdramon();

	//CAM->SetTarget(gumdramon);
}

ReportScene::~ReportScene()
{
	delete land;
	delete jean;
	delete boss;
	delete sky;
	//delete house;
}

void ReportScene::Update() 
{
	land->UpdateWorld();
	jean->Update();
	boss->Update();
	InstancingMonsterManager::Get()->Update();
}

void ReportScene::PreRender()
{
}

void ReportScene::Render()
{
	sky->Render();
	land->Render();
	jean->Render();
	boss->Render();
	InstancingMonsterManager::Get()->Render();
}

void ReportScene::PostRender()
{
	InstancingMonsterManager::Get()->PostRender();
}

void ReportScene::GUIRender()
{
	land->GUIRender();
	jean->GUIRender();
	//boss->GUIRender();
	InstancingMonsterManager::Get()->GUIRender();
	//chuchu->GUIRender();
}

void ReportScene::Start()
{
	InstancingMonsterManager::Get()->CreateMonsters(10.0f);
	InstancingMonsterManager::Get()->SetPlayer(jean);
	InstancingMonsterManager::Get()->SetQuad(land);
}

void ReportScene::End()
{
	//MonsterManager::Delete();
}
