#include "framework.h"
#include "TownScene.h"

TownScene::TownScene()
{
	Siro_Stagenum_008 = new Model("Siro-Stagenum-008");

}

TownScene::~TownScene()
{
	delete Siro_Stagenum_008;
}

void TownScene::Update()
{
	Siro_Stagenum_008->UpdateWorld();
}

void TownScene::PreRender()
{
}

void TownScene::Render()
{
	Siro_Stagenum_008->Render();
}

void TownScene::PostRender()
{
}

void TownScene::GUIRender()
{
	Siro_Stagenum_008->GUIRender();
}

void TownScene::Start()
{
	
}

void TownScene::End()
{
}
