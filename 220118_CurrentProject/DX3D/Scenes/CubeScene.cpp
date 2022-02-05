#include "framework.h"
#include "CubeScene.h"

CubeScene::CubeScene()
{
	/*cube = new Cube();
	cube->GetMaterial()->SetShader(L"Texture.hlsl");
	cube->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");

	cube2 = new Cube();
	cube2->GetMaterial()->SetShader(L"Texture.hlsl");
	cube2->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Bricks.png");

	cube2->position.x = 3.0f;

	cube2->SetParent(cube);*/
	isDown = false;
	robot = new ReportRobot_211201();
}

CubeScene::~CubeScene()
{
	//delete cube;
	//delete cube2;

	delete robot;
}

void CubeScene::Update()
{
	//if(io.KeysDown['W'])
	//	robot->body->position += robot->body->Forward() * DELTA;

	

	// ------------------ 키 조작 ------------------
	if(KEY_PRESS(VK_RIGHT))		// 키 눌려 있을 때
		robot->body->rotation.y += DELTA;

	if (KEY_PRESS(VK_LEFT))
		robot->body->rotation.y -= DELTA;

	if(KEY_PRESS(VK_UP))
		robot->body->position += robot->body->Forward() * DELTA;

	if (KEY_PRESS(VK_DOWN))
		robot->body->position -= robot->body->Forward() * DELTA;

	// ------------------ 마우스 조작 ------------------
	if (MOUSE_PRESS(0))
		robot->body->rotation.y += DELTA;

	ImGuiIO io = ImGui::GetIO();

	ImVec2 delta = io.MouseDelta;

	robot->body->rotation.y += delta.x * DELTA;
	robot->body->rotation.x += delta.y * DELTA;

	robot->body->position.y += io.MouseWheel;
	//if(isDown) robot->body->rotation.y += 100.0f * DELTA;
	/*cube->rotation.x += DELTA;
	cube->rotation.y += DELTA;
	cube2->rotation.y += DELTA;

	cube->UpdateWorld();
	cube2->UpdateWorld();*/

	robot->Update();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
	/*cube->Render();
	cube2->Render();*/
	robot->Render();
}

void CubeScene::PostRender()
{
}

void CubeScene::GUIRender()
{
}
