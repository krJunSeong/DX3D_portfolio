#include "framework.h"
#include "TownScene.h"

TownScene::TownScene()
{
	Siro_Stagenum_008_1 = new Model("Siro-Stagenum-008_1");
	Siro_Stagenum_008_1->Load();
}

TownScene::~TownScene()
{
	delete Siro_Stagenum_008_1;
}

void TownScene::Update()
{
	Siro_Stagenum_008_1->UpdateWorld();
}

void TownScene::PreRender()
{
}

void TownScene::Render()
{
	Siro_Stagenum_008_1->Render();
}

void TownScene::PostRender()
{
}

void TownScene::GUIRender()
{
	Siro_Stagenum_008_1->GUIRender();
}

void TownScene::Start()
{
	float temp = 120.0f / 255.0f;
	Environment::Get()->GetLightBuffer()->data.ambient = {temp, temp, temp};
	Environment::Get()->Load("TownScene.info");
}

void TownScene::End()
{
	Environment::Get()->Save("TownScene.info");
}
