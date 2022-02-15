#include "framework.h"
#include "ReportScene.h"

ReportScene::ReportScene()
{
	// -------------------------- Map --------------------
	land = new Terrain(L"BossMap_Alpha.png", L"BossMap_Height.png");
	sky = new Sky(L"Textures/Landscape/Space.dds");

	leftWall	= new Wall();
	leftWall->tag = "leftWall";
	leftWall->Load();

	upWall		= new Wall();
	upWall->tag = "upWall";
	upWall->Load();

	rightWall	= new Wall();
	rightWall->tag = "rightWall";
	rightWall->Load();

	downWall	= new Wall();
	downWall->tag = "downWall";
	downWall->Load();

	// ----------------------- Character ----------------------
	jean = new Jean();
	jean->SetTerrain(land);

	boss = new RockBoss();
	boss->SetPlayer(jean);
	boss->SetTerrain(land);

	jean->SetRockBoss(boss);

	// CAM
	//CAM->SetTarget(jean);
	
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

	delete leftWall;
	delete upWall;
	delete rightWall;
	delete downWall;

	//delete house;
}

void ReportScene::Update() 
{
	land->UpdateWorld();
	jean->Update();
	boss->Update();

	leftWall	->Update();
	upWall		->Update();
	rightWall	->Update();
	downWall	->Update();


	if(KEY_DOWN(VK_TAB)) Collider::isVisible = !Collider::isVisible;
	
	//InstancingMonsterManager::Get()->Update();
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

	leftWall->Render();
	upWall->Render();
	rightWall->Render();
	downWall->Render();
	//InstancingMonsterManager::Get()->Render();
}

void ReportScene::PostRender()
{
	//InstancingMonsterManager::Get()->PostRender();
	boss->PostRender();
}

void ReportScene::GUIRender()
{
	land->GUIRender();
	jean->GUIRender();
	boss->GUIRender();

	leftWall->GUIRender();
	upWall->GUIRender();
	rightWall->GUIRender();
	downWall->GUIRender();

	//InstancingMonsterManager::Get()->GUIRender();
	
	//boss->GUIRender();
	//chuchu->GUIRender();
}

void ReportScene::Start()
{
	//InstancingMonsterManager::Get()->CreateMonsters(10.0f);
	//InstancingMonsterManager::Get()->SetPlayer(jean);
	//InstancingMonsterManager::Get()->SetQuad(land);
}

void ReportScene::End()
{
	//MonsterManager::Delete();
}
