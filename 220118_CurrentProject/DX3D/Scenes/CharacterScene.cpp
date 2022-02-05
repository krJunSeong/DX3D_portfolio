#include "framework.h"
#include "CharacterScene.h"

CharacterScene::CharacterScene()
{
	//zombie = new Zombie();
	//zombie->GetReader()->SetShader(L"Lighting.hlsl");

	terrain = new Terrain();
	sky = new Sky(L"Textures/Landscape/Space.dds");

	quad = new Quad();
	quad->tag = "Cursor";
	quad->GetMaterial()->SetShader(L"Texture.hlsl");
	quad->GetMaterial()->SetDiffuseMap(L"Textures/UI/cursor.png");
	quad->Load();

	//gumdramon = new Gumdramon();
	chuchu = new Chuchu();

	horse = new Horse();
}

CharacterScene::~CharacterScene()
{
	//delete zombie;
	delete terrain;
	delete sky;
	delete chuchu;
	delete quad;

	delete horse;
	delete gumdramon;
}

void CharacterScene::Update()
{
	quad->position.x = mousePos.x;
	quad->position.y = WIN_HEIGHT - mousePos.y;
	//quad->position = CAM->WorldToScreenPoint(mousePos);

	chuchu->Update();
	//zombie->Update();

	quad->UpdateWorld();

	horse->Update();
	//gumdramon->Update();
}

void CharacterScene::PreRender()
{
	
}

void CharacterScene::Render()
{
	sky->Render();
	chuchu->Render();
	//zombie->Render();
	horse->Render();
	//gumdramon->Render();
	terrain->Render();
}

void CharacterScene::PostRender()
{
	quad->Render();
	//zombie->PostRender();
}

void CharacterScene::GUIRender()
{
	horse->GUIRender();
	//zombie->GUIRender();
	chuchu->GUIRender();
	quad->GUIRender();
	//gumdramon->GUIRender();
	//gumdramon->collider->GUIRender();
}
