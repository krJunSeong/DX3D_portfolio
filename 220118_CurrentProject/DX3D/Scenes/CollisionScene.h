#pragma once

class CollisionScene : public Scene
{
	Collider* collider;
	vector<Collider*> colliders;
public:
	CollisionScene();
	~CollisionScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};