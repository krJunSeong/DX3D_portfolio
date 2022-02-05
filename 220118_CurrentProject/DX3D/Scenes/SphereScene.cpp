#include "framework.h"
#include "SphereScene.h"

SphereScene::SphereScene()
{
	sphere = new Sphere();
	sphere->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	sphere->GetMaterial()->SetSpecularMap(L"Textures/Landscape/fieldstone_SM.tga");
	sphere->GetMaterial()->SetNormalMap(L"Textures/Landscape/fieldstone_NM.tga");

	sphere->scale *= 10;

	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Dirt.png");

	cube->position.x += 20.0f;
	cube->scale *= 10;
	cube->rotation.x = 2.0f;
}

SphereScene::~SphereScene()
{
	delete sphere;
	delete cube;
}

void SphereScene::Update()
{
	sphere->rotation.y += DELTA;
	sphere->UpdateWorld();

	cube->rotation.y += DELTA;
	cube->UpdateWorld();
}

void SphereScene::PreRender()
{
}

void SphereScene::Render()
{
	sphere->Render();
	cube->Render();
}

void SphereScene::PostRender()
{
}

void SphereScene::GUIRender()
{
	sphere->GetMaterial()->GUIRender();
}

/*
	빛 같은 거 받으면 정점마다 연산을 한다.
	smooth어쩌고 하는 것도 정점 수 줄여서 좀 더 자연스럽게 해준 것임.

	RS
		normal 자체로 RS에 넘겨주면, 중간에 있는 보간값이 진행된다.

		원래 점 3개 이용해서 보간 때린거랑

		그 점 3개사이에 세세하게 찍어서 보간찍은거랑 차이가 난다.
*/
