#include "Framework.h"

Camera::Camera()
    : moveSpeed(100.0f), rotSpeed(10.0f), wheelSpeed(10.0f), target(nullptr),
    distance(20.0f), height(20.0f), moveDamping(5.0f), rotDamping(0.0f),
    rotY(0.0f), destRot(0.0f)
{
    viewBuffer = new ViewBuffer();
    view = XMMatrixIdentity();

    Load();
}

Camera::~Camera()
{
    Save();

    delete viewBuffer;
}

void Camera::Update()
{
    if (target)
        FollowMode();
    else
        FreeMode();
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("CameraOption"))
    {
        ImGui::Indent();
        ImGui::InputFloat3("CamPos", (float*)&position, "%.0f");
        ImGui::InputFloat3("CamRot", (float*)&rotation, "%.0f");
        ImGui::SliderFloat("CamMoveSpeed", &moveSpeed, 20.0f, 200.0f);
        ImGui::SliderFloat("CamRotSpeed", &rotSpeed, 1.0f, 50.0f);
        ImGui::SliderFloat("CamWheelSpeed", &wheelSpeed, 10.0f, 1000.0f);
        ImGui::Unindent();

        if (target && ImGui::TreeNode("TargetOption"))
        {
            ImGui::DragFloat("Distance", &distance, 0.1f);
            ImGui::DragFloat("Height", &height, 0.1f);
            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);
            float degree = XMConvertToDegrees(rotY);
            ImGui::DragFloat("RotationY", &degree, 1.0f, 0, 360);
            rotY = XMConvertToRadians(degree);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f, 0, 100);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f, 0, 100);

            ImGui::TreePop();
        }

        ImGui::TreePop();
    }
}

void Camera::SetBuffer(UINT slot)
{
    viewBuffer->SetVSBuffer(slot);
    viewBuffer->SetGSBuffer(slot);
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
    Float2 screenSize(WIN_WIDTH, WIN_HEIGHT);

    Float2 point;
    point.x = (2.0f * (screenPos.x / screenSize.x)) - 1.0f;
    point.y = ((2.0f * (screenPos.y / screenSize.y)) - 1.0f) * -1.0f;

    Matrix projection = Environment::Get()->GetProjection();

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

    point.x /= temp._11;
    point.y /= temp._22;

    Ray ray;
    ray.position = position;

    Vector3 tempPos(point.x, point.y, 1.0f);

    Matrix invView = XMMatrixInverse(nullptr, view);
    ray.direction = XMVector3TransformNormal(tempPos, invView);
    ray.direction.Normalize();

    return ray;
}

Vector3 Camera::WorldToScreenPoint(Vector3 worldPos)
{
    Vector3 screenPos;

    screenPos = XMVector3TransformCoord(worldPos, view);
    screenPos = XMVector3TransformCoord(screenPos, Environment::Get()->GetProjection());
    //NDC��ǥ : (-1~1) -> 0~WIDTH

    screenPos = (screenPos + 1.0f) * 0.5f;//(0~1)

    screenPos.x *= WIN_WIDTH;
    screenPos.y *= WIN_HEIGHT;

    return screenPos;
}

void Camera::SetTargetTransform()
{
    destRot = target->rotation.y + XM_PI;
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3(0, 0, 1), rotMatrix);

    target->UpdateWorld();
    //position = target->GlobalPos();
    position = forward * -distance + target->GlobalPos();
    position.y += height;
}

void Camera::FreeMode()
{
    if (MOUSE_PRESS(1))
    {
        if (KEY_PRESS('W'))
            position += Forward() * moveSpeed * DELTA;
        if (KEY_PRESS('S'))
            position -= Forward() * moveSpeed * DELTA;
        if (KEY_PRESS('D'))
            position += Right() * moveSpeed * DELTA;
        if (KEY_PRESS('A'))
            position -= Right() * moveSpeed * DELTA;
        if (KEY_PRESS('E'))
            position += Up() * moveSpeed * DELTA;
        if (KEY_PRESS('Q'))
            position -= Up() * moveSpeed * DELTA;

        ImVec2 delta = ImGui::GetIO().MouseDelta;

        rotation.x += delta.y * rotSpeed * DELTA;
        rotation.y += delta.x * rotSpeed * DELTA;
    }

    position += Forward() * wheelSpeed * ImGui::GetIO().MouseWheel;

    UpdateWorld();
    view = XMMatrixInverse(nullptr, world);
    SetView();
}

void Camera::FollowMode()
{
    ImGuiIO io = ImGui::GetIO();
    distance += io.MouseWheel * wheelSpeed;

    destRot = LERP(destRot, target->rotation.y + XM_PI, rotDamping * DELTA);
    rotation.y = destRot + rotY;
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3(0, 0, 1), rotMatrix);

    destPos = forward * -distance + target->GlobalPos();
    destPos.y += height;

    position = LERP(position, destPos, moveDamping * DELTA);


    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);

    view = XMMatrixLookAtLH(position, target->position + offset, Vector3(0, 1, 0));

    SetView();
}

void Camera::SetView()
{
    //Vector3 focus = position + Forward();
    //view = XMMatrixLookAtLH(position, focus, Up());

    viewBuffer->Set(view);
}

void Camera::Save()
{
    BinaryWriter w("TextData/OptionData/Camera.info");

    w.Vector(position);
    w.Vector(rotation);

    w.Float(moveSpeed);
    w.Float(rotSpeed);

    w.Float(distance);
    w.Float(height);
    w.Float(rotY);
    w.Vector(focusOffset);
    w.Float(moveDamping);
    w.Float(rotDamping);
}

void Camera::Load()
{
    BinaryReader r("TextData/OptionData/Camera.info");

    if (r.IsFailed()) return;

    position = r.Vector();
    rotation = r.Vector();

    moveSpeed = r.Float();
    rotSpeed = r.Float();

    distance = r.Float();
    height = r.Float();
    rotY = r.Float();
    focusOffset = r.Vector();
    moveDamping = r.Float();
    rotDamping = r.Float();
}



/*
#include "framework.h"

Camera::Camera() : moveSpeed(100.0f), rotSpeed(10.0f)
{
	viewBuffer = new ViewBuffer();
	view = XMMatrixIdentity();		// view ��� �ʱ�ȭ

	Load();
}

Camera::~Camera()
{
	Save();

	delete viewBuffer;
}

void Camera::Update()
{
	FreeMode();
}

void Camera::GUIRender()
{
	if(ImGui::TreeNode("CameraOption"))
	{
		ImGui::Indent();
		ImGui::InputFloat3("CamPos", (float*)&position, "%.1f");
		ImGui::SliderFloat("CamMoveSpeed", &moveSpeed, 1.0f, 100.0f);
		ImGui::SliderFloat("CamRotSpeed", &rotSpeed, 1.0f, 50.0f);
		ImGui::Unindent();

		ImGui::TreePop();
	}
}

void Camera::SetBuffer()
{
	viewBuffer->SetVSBuffer(1);
}

Ray Camera::ScreenPointToRay(Vector3 screenPos)
{
	Float2 screenSize(WIN_WIDTH, WIN_HEIGHT);

	Float2 point; // screen ��ǥ�� NDC ��ǥ�� �ٲ�
	point.x = (2 * (screenPos.x / screenSize.x)) - 1.0f;
	point.y = ((2 * (screenPos.y / screenSize.y)) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get()->GetProjection();

	Float4x4 temp;
	XMStoreFloat4x4(&temp, projection);

	// X, y�� ���� ������ ����ȭ ��Ű�� ����.
	point.x /= temp._11;
	point.y /= temp._22;

	Ray ray;
	ray.position = position; // ī�޶���ġ���� ���.

	Vector3 tempPos(point.x, point.y , 1.0f); // ī�޶󺸴� z�� 1 �� ��

	ray.direction = XMVector3TransformNormal(tempPos, world);
	ray.direction.Normalize(); 

	return ray;
}

void Camera::FreeMode()
{
	if(MOUSE_PRESS(1))
	{
		if (KEY_PRESS('W')) position += Forward() * moveSpeed * DELTA;
		else if (KEY_PRESS('S')) position -= Forward() * moveSpeed * DELTA;
		else if (KEY_PRESS('D')) position += Right() * moveSpeed * DELTA;
		else if (KEY_PRESS('A')) position -= Right() * moveSpeed * DELTA;
		else if (KEY_PRESS('E')) position -= Up() * moveSpeed * DELTA;
		else if (KEY_PRESS('Q')) position += Up() * moveSpeed * DELTA;

		ImVec2 delta = ImGui::GetIO().MouseDelta;

		rotation.x += delta.y * rotSpeed * DELTA;
		rotation.y += delta.x * rotSpeed * DELTA;
	}

	UpdateWorld();
	SetView();
}

void Camera::SetView()

{
    /*
	// viewBuffer�� ����� �θ�� �����
	// �갡 D ������ �������� �̵��Ѵٰ� �ϸ�, ������ ��� ��ü���� ���������� �̵���.
	// �����״� ���� �������� �̵��ϴ� �����̰���
	// �׷��� ����İ��� �༭ ���� �������� ���� �ϸ� ����� �������� ������ ����� ����.
	// ��, ���� �����Ϸ��� �ϴ� ���� �ݴ�������� ����� ��������.
	

	// ---------- ��� 1 ( LookAt)-----------------
	//Vector3 focus = position + Forward();
	//view = XMMatrixLookAtLH(position, focus, Up());
	//viewBuffer->Set(view);

	// ----------- ��� 2 (�����)---------------
	view = XMMatrixInverse(nullptr, world);
	viewBuffer->Set(view, world);
}

void Camera::Save()
{
	// Banary�� �ڿ� Ȯ���� �������.
	BinaryWriter w(L"TextData/OptionData/Camera.info");

	w.Vector(position);
	w.Vector(rotation);

	w.Float(moveSpeed);
	w.Float(rotSpeed);
}

void Camera::Load()
{
	BinaryReader r(L"TextData/OptionData/Camera.info");

	if(r.IsFailed()) return;
	
	position = r.Vector();
	rotation = r.Vector();

	moveSpeed = r.Float();
	rotSpeed = r.Float();
}
*/