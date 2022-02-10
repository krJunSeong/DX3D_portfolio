#pragma once

class ReportCamera_211205 : public Transform
{
private:
	MatrixBuffer* viewBuffer;		// view�� ���߿� �� ����. ī�޶�� ��� �ϴϱ�. �ϴ��� ���� ���ô�.

	Vector4 eye;
	Vector4 focus;
	Vector4 up;

	Transform* target;
	//	ImGuiIO io;
	//	ImVec2 delta;

public:
	ReportCamera_211205();
	~ReportCamera_211205();

	void Set(UINT slot = 1);
	void Render();
};
