#include "framework.h"

ReportCamera_211205::ReportCamera_211205()
{
	viewBuffer = new MatrixBuffer();
	//io = ImGui::GetIO();
	//delta = io.MouseDelta;

	position = { 7, 10, -20 };
	eye = XMVectorSet(position.x, position.y, position.z, 0); //카메라 위치
	focus = XMVectorSet(0, 0, 5, 0);  //카메라 비추는 방향
	up = XMVectorSet(0, 1, 0, 0);

	Matrix view = XMMatrixLookAtLH(eye, focus, up);


	viewBuffer->Set(view);
}

ReportCamera_211205::~ReportCamera_211205()
{
	delete viewBuffer;
}

void ReportCamera_211205::Render()
{
	//delta = ImGui::GetIO().MouseDelta;
	// Left: 0, right: 1
	if (MOUSE_PRESS(1))
	{
		if (KEY_PRESS('W')) position.z += DELTA;
		else if (KEY_PRESS('S')) position.z -= DELTA;
		else if (KEY_PRESS('A')) position.x -= DELTA;
		else if (KEY_PRESS('D')) position.x += DELTA;
		else if (KEY_PRESS('Q')) position.y += DELTA;
		else if (KEY_PRESS('E')) position.y -= DELTA;

		ImVec2 delta = ImGui::GetIO().MouseDelta;

		rotation.x += delta.x * 0.05f;
		rotation.y -= delta.y * 0.05f;
	}

	if (abs(ImGui::GetIO().MouseWheel) >= 0.1f)
		position += Forward() * ImGui::GetIO().MouseWheel;

	//ImGui::Text("Position(X : %d, Y : %d, Z : %d)", (int)position.x, (int)position.y, (int)position.z);
	//ImGui::Text("Position(X : %d, Y : %d, Z : %d)", (int)position.x, (int)position.y, (int)position.z);

	Set();
}

void ReportCamera_211205::Set(UINT slot)
{
	eye = XMVectorSet(position.x, position.y, position.z, 0); // 카메라 위치

	Vector3 targetPos;
	if (target == nullptr) targetPos = { 0,0,0 };
	else targetPos = target->Forward();

	focus = XMVectorSet(rotation.x, rotation.y, rotation.z, 0);	  // 카메라 비추는 방향
	up = XMVectorSet(0, 1, 0, 0);

	Matrix view = XMMatrixLookAtLH(eye, focus, up);

	viewBuffer->Set(view);
	viewBuffer->SetVSBuffer(slot);
}