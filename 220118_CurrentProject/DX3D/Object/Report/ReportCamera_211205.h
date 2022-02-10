#pragma once

class ReportCamera_211205 : public Transform
{
private:
	MatrixBuffer* viewBuffer;		// view는 나중에 뺄 것임. 카메라로 써야 하니까. 일단은 같이 갑시다.

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
