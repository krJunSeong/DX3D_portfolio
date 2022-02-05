#pragma once

class Scene
{
public:
	virtual ~Scene() = default; // ��ĳ����, ĳ����, �ڽ�: ������ �θ�͸� ȣ���ϰ� �ڽİ� ȣ�����. �׷��� ����� ������

	virtual void Update() = 0;
	
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;	// ui
	virtual void GUIRender() = 0;	// img

	virtual void Start() {}
	virtual void End() {}
};
