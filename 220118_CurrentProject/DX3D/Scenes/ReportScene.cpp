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

	backLand = new Quad();
	backLand->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Grace.png");
	backLand->tag = "backLand";
	backLand->Load();
	// ----------------------- Character ----------------------
	jean = new Jean();
	jean->SetTerrain(land);

	boss = new RockBoss();
	boss->SetPlayer(jean);
	boss->SetTerrain(land);

	jean->SetRockBoss(boss);

	// -------------------------------------------------------
	startPos = new BoxCollider();
	startPos->tag = "bossRoomStartPos";
	startPos->Load();

	bossStartPos = new BoxCollider();
	bossStartPos->tag = "bossStartPos";
	bossStartPos->Load();

	//CAM->position.y += 50.0f;
	//CAM->position.z += 500.0f;

	//CAM->SetTarget(gumdramon);

	// -------------------- Test Particle
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

	delete startPos;

	delete backLand;
}

void ReportScene::Update() 
{
	//if(KEY_DOWN('O'))
	//	Environment::Get()->Save("ReportScene.info");
	//
	//if (KEY_DOWN('P'))
	//	Environment::Get()->Load("ReportScene.info");

	land->UpdateWorld();
	jean->Update();
	boss->Update();
	backLand->UpdateWorld();

	leftWall	->Update();
	upWall		->Update();
	rightWall	->Update();
	downWall	->Update();

	startPos->UpdateWorld();
	bossStartPos->UpdateWorld();

	if(KEY_DOWN(VK_TAB)) Collider::isVisible = !Collider::isVisible;

	//if (MOUSE_CLICK(0)) 
	//	ParticleManager::Get()->Play("UpStar", jean->position, 0);

	LightContorll();

	//int count = 0;
	//for (auto pillar : boss->GetRockPillares())
	//{
	//	if (pillar->isActive)
	//		ParticleManager::Get()->Play("UpStar", pillar->position, count);
	//
	//	//else
	//	//	ParticleManager::Get()->Stop("UpStar", count);
	//
	//	count++;
	//}

	//if (MOUSE_CLICK(1))   CAM->SetTarget(nullptr);
	//else if (MOUSE_UP(1)) CAM->SetTarget(jean);

	if(KEY_PRESS('0')) CAM->SetTarget(nullptr);

	ParticleManager::Get()->Update();


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
	//backLand->Render();
	
	// ---------------- wall ----------------
	leftWall->Render();
	upWall->Render();
	rightWall->Render();
	downWall->Render();

	// ------------------- Pos ---------------
	startPos->Render();
	bossStartPos->Render();

	// ---------------- Particle --------------------
	ParticleManager::Get()->Render();	
	//InstancingMonsterManager::Get()->Render();
}

void ReportScene::PostRender()
{
	//InstancingMonsterManager::Get()->PostRender();
	boss->PostRender();
	jean->PostRender();
}

void ReportScene::GUIRender()
{
	land->GUIRender();
	jean->GUIRender();
	boss->GUIRender();

	backLand->GUIRender();
	leftWall->GUIRender();
	upWall->GUIRender();
	rightWall->GUIRender();
	downWall->GUIRender();

	startPos->GUIRender();
	bossStartPos->GUIRender();
	//InstancingMonsterManager::Get()->GUIRender();
	
	//boss->GUIRender();
}

void ReportScene::Start()
{
	srand(time(NULL));

	//InstancingMonsterManager::Get()->CreateMonsters(10.0f);
	//InstancingMonsterManager::Get()->SetPlayer(jean);
	//InstancingMonsterManager::Get()->SetTerrain(land);

	jean->position = startPos->position;
	CAM->SetTarget(jean);

	boss->SetBossPosition(bossStartPos->position);

	// --------------- Map Light -------------------------
	float temp = 50.0f / 255;
	Environment::Get()->GetLightBuffer()->data.ambient = {temp, temp, temp};

	// --------------------- AddLight shield, pillar4, Player
	Environment::Get()->GetLightBuffer()->data.lightCount++;
	Environment::Get()->GetLightBuffer()->data.lightCount++;
	Environment::Get()->GetLightBuffer()->data.lightCount++;
	Environment::Get()->GetLightBuffer()->data.lightCount++;
	Environment::Get()->GetLightBuffer()->data.lightCount++;

	for(int i = 0; i < 6; i++)
	{
		Environment::Get()->GetLightBuffer()->data.lights[i].type = LightType::POINT;
	}

	ParticleManager::Get()->CreateParticle();
}

void ReportScene::End()
{
	//MonsterManager::Delete();
}

void ReportScene::LightContorll()
{
	// rockPillar
	int count = 0;
	for(auto pillar : boss->GetRockPillares())
	{
		Environment::Get()->GetLightBuffer()->data.lights[count].active =  pillar->isActive;
		Environment::Get()->GetLightBuffer()->data.lights[count].position = pillar->position;
		Environment::Get()->GetLightBuffer()->data.lights[count].position.y = pillar->position.y + 4.0f;

		count++;
	}

	// RcokShield
	Environment::Get()->GetLightBuffer()->data.lights[count].active = boss->GetRcokShield()->isActive;
	Environment::Get()->GetLightBuffer()->data.lights[count].position = boss->GetRcokShield()->position;
	Environment::Get()->GetLightBuffer()->data.lights[count].position.y = 4.0f;
	count++;

	// player
	Environment::Get()->GetLightBuffer()->data.lights[count].active = jean->isActive;
	Environment::Get()->GetLightBuffer()->data.lights[count].position = jean->position;
	Environment::Get()->GetLightBuffer()->data.lights[count].position.y = jean->position.y + 4.0f;
	count++;
}
