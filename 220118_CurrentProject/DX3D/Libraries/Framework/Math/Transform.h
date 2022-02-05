#pragma once

class Transform
{
public:
    string tag;

    bool isActive;
    bool isFrustum;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

protected:
    Vector3 globalPosition;
    Vector3 globalRotation;
    Vector3 globalScale;

    Vector3 pivot;

    Matrix world;
    Matrix* parent;

public:
    Transform();

    void UpdateWorld(bool isQuat = false);

    void GUIRender();

    Vector3 Forward();
    Vector3 Up();
    Vector3 Right();

    Vector3 GlobalPos() { return globalPosition; }
    Vector3 GlobalScale() { return globalScale; }

    Matrix& GetWorld() { return world; }

    void SetParent(Matrix* parent) { this->parent = parent; }
    void SetParent(Transform* transform) { parent = &transform->GetWorld(); }

    void SetPivot(float x, float y, float z) { pivot = { x, y, z }; }

    void Save();
    void Load();
};

/* 12.1 01:22:00  �Ǻ��� ��� ����.
  ���� ���� �������� �������� �ٸ� ���鸸 ������ �ʹٸ�?
 
	��Ȳ: ����: 0.0

	1. �������� �ϴ� ���� ����ķ� �̵���Ų��.(�»���� �����, ���ϴ��� ���� ���)
	2. �׷� �ش� ������ �߽����� ������ ���ġ�ȴ�.
	3. �� ���¿��� �����̼� ������ �����̼� ��Ʈ������ ���� 0.0�� �������� �ν��ϰ� ������ ������
	  ���ġ�� ���¿����� �������� ������, ������ �͵��� ���ư���.
	4. �׸��� �ٽ� �ǵ�����.(������� ����ķ�, �� �»�� ��ķ�)

	�Ǻ��� ������� ���� ���ְ� SRT ���ְ� P�� �ٽ� ������
	IP * S * R * T * P
*/