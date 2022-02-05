#include "Framework.h"
#include "DefenceScene.h"

DefenceScene::DefenceScene()
{
	sky = new Sky(L"Textures/Landscape/Snow_ENV.dds");
	terrain = new Terrain();

	startPos = new SphereCollider();
	startPos->Load();
}

DefenceScene::~DefenceScene()
{
	delete sky;
	delete terrain;
	delete startPos;
	
	if(amber) delete amber;
}

void DefenceScene::Update()
{
	amber->Update();

	startPos->UpdateWorld();
}

void DefenceScene::PreRender()
{

}

void DefenceScene::Render()
{
	sky->Render();
	terrain->Render();

	amber->Render();

	startPos->Render();
}

void DefenceScene::PostRender()
{
	amber->PostRender();
}

void DefenceScene::GUIRender()
{
	amber->GUIRender();
	startPos->GUIRender();
}

void DefenceScene::Start()
{
	amber = new Amber();
	amber->position = startPos->position;
	amber->rotation = startPos->rotation;

	CAM->SetTarget(amber);
	CAM->position = amber->position;
	CAM->rotation = amber->rotation;
}

void DefenceScene::End()
{
	delete amber;
}
