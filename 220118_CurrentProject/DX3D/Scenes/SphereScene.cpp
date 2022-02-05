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
	�� ���� �� ������ �������� ������ �Ѵ�.
	smooth��¼�� �ϴ� �͵� ���� �� �ٿ��� �� �� �ڿ������� ���� ����.

	RS
		normal ��ü�� RS�� �Ѱ��ָ�, �߰��� �ִ� �������� ����ȴ�.

		���� �� 3�� �̿��ؼ� ���� �����Ŷ�

		�� �� 3�����̿� �����ϰ� �� ���������Ŷ� ���̰� ����.
*/
