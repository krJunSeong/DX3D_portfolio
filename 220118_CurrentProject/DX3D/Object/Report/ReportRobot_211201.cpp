#include "framework.h"

ReportRobot_211201::ReportRobot_211201(Float3 position)
	:state(State::IDLE), partsSize(1.0f, 1.0f, 1.0f), isFoward(true), temp(1.0f)
{
	cube = new Cube();
	cube->GetMaterial()->SetShader(L"Texture.hlsl");
	cube->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");

	cube2 = new Cube();
	cube2->GetMaterial()->SetShader(L"Texture.hlsl");
	cube2->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Bricks.png");

	cube2->position.x = 3.0f;

	cube2->SetParent(cube);

	head	 = new Cube(partsSize);
	body	 = new Cube(Float3(1.0f, 2.0f, 1.0f));
	leftArm	 = new Cube(partsSize);
	rightArm = new Cube(partsSize);
	leftLeg  = new Cube(partsSize);
	rightLeg = new Cube(partsSize);

	head	->GetMaterial()->SetShader(L"Texture.hlsl");
	body	->GetMaterial()->SetShader(L"Texture.hlsl");
	leftArm	->GetMaterial()->SetShader(L"Texture.hlsl");
	rightArm->GetMaterial()->SetShader(L"Texture.hlsl");
	leftLeg ->GetMaterial()->SetShader(L"Texture.hlsl");
	rightLeg->GetMaterial()->SetShader(L"Texture.hlsl");

	head	->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Bricks.png");
	body	->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Bricks.png");
	leftArm	->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
	rightArm->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
	leftLeg	->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");
	rightLeg->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Box.png");

	body    ->SetParent(this);
	body->position = position;
	head	->SetParent(body);
	leftArm	->SetParent(head);
	rightArm->SetParent(head);
	leftLeg	->SetParent(body);
	rightLeg->SetParent(body);

	head->position.y = 1.5f;

	//body->rotation.y = XM_PI;
	leftArm->position.x = -1.0f;
	body->position.y =2.5f;
	rightArm->position.x = +1.0f;
	
	leftLeg->position.y = -1.5f;
	leftLeg->position.x = -0.7f;

	rightLeg->position.y = -1.5f;
	rightLeg->position.x = +0.7f;

	leftLeg->SetPivot(0, 0.5f, 0);
	rightLeg->SetPivot(0, 0.5f, 0);
	
	leftArm->SetPivot(0.5f, 0, 0);
	rightArm->SetPivot(-0.5f, 0, 0);
}

ReportRobot_211201::~ReportRobot_211201()
{
	delete cube;
	delete cube2;

	delete	head	;
	delete	body	;
	delete	leftArm	;
	delete	rightArm;
	delete	leftLeg	;
	delete	rightLeg;
}

void ReportRobot_211201::Update()
{	
	static float angleY = 0.0f;
	static float angleX = 0.0f;
	static float temp = 1.0f;

	if (GetAsyncKeyState('W'))
	{
		body->position.z += 10 * DELTA;
		body->rotation.y = 0;
	}
	else if (GetAsyncKeyState('S'))
	{
		body->position.z -= 10 * DELTA;
		body->rotation.y = XM_PI;
	}
	else if (GetAsyncKeyState('D'))
	{
		body->position.x += 10 * DELTA;
		body->rotation.y = XM_PIDIV2;
	}
	else if (GetAsyncKeyState('A'))
	{
		body->position.x -= 10 * DELTA;
		body->rotation.y =  XM_PI + XM_PIDIV2;
	}

	if(GetAsyncKeyState(VK_HOME))
	{
		state = State::IDLE;
		leftLeg->rotation.x = 0;
		rightArm->rotation.x = 0;
		rightLeg->rotation.x = 0;
		leftArm->rotation.x = 0;
	}

	else if(GetAsyncKeyState(VK_END)) 
	{
		state = State::WALK;
		rightArm->position.y = 0;					
		leftArm->position.y = 0;
	}
	
	switch (state)
	{
	case ReportRobot_211201::State::IDLE:
		if(abs(body->position.x - leftArm->position.y) >= 1.0f)
			temp *= -1;

		head->rotation.y += 10.0f * DELTA;
		leftArm->position.y += temp * DELTA;
		rightArm->position.y += temp * DELTA;
		break;

	case ReportRobot_211201::State::WALK:

		if(abs(leftLeg->rotation.x) >= XM_PIDIV4) temp *= -1;

		leftLeg->rotation.x += temp * DELTA;
		rightArm->rotation.x += temp * DELTA;

		rightLeg->rotation.x += -temp * DELTA;
		leftArm->rotation.x  += -temp * DELTA;

		/*
		angleX += 10.0f * DELTA;

		leftLeg-> rotation.x = sin(angleX);
		rightArm->rotation.x = sin(angleX);

		rightLeg->rotation.x = -sin(angleX);
		leftArm->rotation.x	 = -sin(angleX);
		*/
		break;

	}

	head	->UpdateWorld();
	body	->UpdateWorld(); 
	leftArm	->UpdateWorld();
	rightArm->UpdateWorld();
	leftLeg	->UpdateWorld();
	rightLeg->UpdateWorld();

	UpdateWorld();
}

void ReportRobot_211201::GUIRender()
{
	/*if (ImGui::TreeNode("TerrainEditor"))
	{
		if (ImGui::Button("CreateObject")) Update();
		ImGui::SameLine();

		ImGui::TreePop();
	}*/
}

void ReportRobot_211201::UpdateTerrainUp(vector<VertexUV> vertices, UINT width, UINT height)
{	
	// 소수점 자르기
	int intPosX = body->position.x;
	int intPosZ = body->position.z;
	//int index = (width * intPosZ) + intPosX;
	int index = height * (height - intPosZ - 1) + intPosX;
	// body
	body->position.y = vertices[index].position.y + 2.0f; 
}

void ReportRobot_211201::Render()
{
	head	  ->Render();
	body	  ->Render();
	leftArm	  ->Render();
	rightArm  ->Render();
	leftLeg	  ->Render();
	rightLeg  ->Render();
	GUIRender();

}

