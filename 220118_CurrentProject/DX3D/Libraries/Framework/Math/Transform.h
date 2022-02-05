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

/* 12.1 01:22:00  피봇을 잡는 원리.
  만약 내가 왼쪽위를 중점으로 다른 점들만 돌리고 싶다면?
 
	상황: 중점: 0.0

	1. 잡으려고 하는 곳의 역행렬로 이동시킨다.(좌상단의 역행렬, 우하단을 향한 행렬)
	2. 그럼 해당 지점을 중심으로 정점이 재배치된다.
	3. 그 상태에서 로테이션 돌리면 로테이션 매트릭스가 원래 0.0을 중점으로 인식하고 돌리기 때문에
	  재배치된 상태에서의 왼쪽위는 가만히, 나머지 것들은 돌아간다.
	4. 그리고 다시 되돌린다.(역행렬의 역행렬로, 즉 좌상단 행렬로)

	피봇의 역행렬을 먼저 해주고 SRT 해주고 P로 다시 돌리고
	IP * S * R * T * P
*/