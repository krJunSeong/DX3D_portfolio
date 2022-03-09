#include "framework.h"
#include "ReportScene.h"

ReportScene::ReportScene()
{
	// -------------------------- Map --------------------
	land = new Terrain(L"BossMap_Alpha.png", L"BossMap_Height.png");
	sky = new Sky(L"Textures/Landscape/Space.dds");
	rain = new Rain();

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

	// -------------------- position ----------------------
	startPos = new BoxCollider();
	startPos->tag = "bossRoomStartPos";
	startPos->Load();

	bossStartPos = new BoxCollider();
	bossStartPos->tag = "bossStartPos";
	bossStartPos->Load();

	//CAM->position.y += 50.0f;
	//CAM->position.z += 500.0f;
	// -------------------- Test Particle
}

ReportScene::~ReportScene()
{
	delete land;
	delete jean;
	delete boss;
	delete sky;
	delete rain;

	delete leftWall;
	delete upWall;
	delete rightWall;
	delete downWall;

	delete startPos;

	delete backLand;
}

void ReportScene::Phase2MapSetting()
{
}

void ReportScene::Update()
{
	land->UpdateWorld();
	jean->Update();
	backLand->UpdateWorld();

	leftWall	->Update();
	upWall		->Update();
	rightWall	->Update();
	downWall	->Update();

	startPos->UpdateWorld();
	bossStartPos->UpdateWorld();

	if(KEY_DOWN(VK_TAB)) Collider::isVisible = !Collider::isVisible;

	if (MOUSE_CLICK(1))   CAM->SetTarget(nullptr);
	else if (MOUSE_UP(1)) CAM->SetTarget(jean);
	
	if(InstancingMonsterManager::Get()->GetKillCount() > 3) isBossPhase = true;
	
	if(isBossPhase)
	{
		LightContorll();
		rain->Update();
		boss->Update();
	}

	else InstancingMonsterManager::Get()->Update();

	ParticleManager::Get()->Update();
}

void ReportScene::PreRender()
{
}

void ReportScene::Render()
{
	sky->Render();
	land->Render();

	jean->Render();
	
	backLand->Render();
	
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

	if (isBossPhase) {rain->Render(); boss->Render();}
	else InstancingMonsterManager::Get()->Render();
}

void ReportScene::PostRender()
{
	if (isBossPhase) boss->PostRender(); 
	else InstancingMonsterManager::Get()->PostRender();

	jean->PostRender();
}

void ReportScene::GUIRender()
{
	land->GUIRender();
	rain->GUIRender();
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

	InstancingMonsterManager::Get()->CreateMonsters(10);
	InstancingMonsterManager::Get()->SetPlayer(jean);
	InstancingMonsterManager::Get()->SetTerrain(land);

	jean->position = startPos->position;
	//CAM->SetTarget(jean);

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
	rain->Play({0,0,0});
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
