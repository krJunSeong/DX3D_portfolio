#include "framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain();
	//robot = new ReportRobot_211201();
	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/box.png");
}

TerrainScene::~TerrainScene()
{
	//delete robot;
	delete terrain;
	delete cube;
}

void TerrainScene::Update()
{
	//robot->Update();
	//robot->UpdateTerrainUp(terrain->GetVertices(), terrain->GetWidth(), terrain->GetHeight());

	if(KEY_PRESS('W'))
		cube->position += cube->Forward() * 10.0f * DELTA;
	if (KEY_PRESS('A'))
		cube->rotation.y -= DELTA;
	if (KEY_PRESS('D'))
		cube->rotation.y += DELTA;

	cube->position.y = terrain->GetHeight(cube->position);
	cube->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	//robot->Render();
	terrain->Render();
	cube->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
}
