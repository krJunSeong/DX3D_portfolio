#pragma once

class Ray;

class Camera : public Transform
{
private:
    float moveSpeed;
    float rotSpeed;
    float wheelSpeed;
    float duration = 0.0f;
    float magnitude;

    Vector3 originPos;

    ViewBuffer* viewBuffer;

    Matrix view;

    Transform* target;

    float distance;
    float height;

    float rotY;

    float destRot;
    Vector3 destPos;
    Vector3 focusOffset;

    Matrix rotMatrix;

    float moveDamping;
    float rotDamping;
public:
    Camera();
    ~Camera();

    void Update();
    void GUIRender();

    void SetBuffer(UINT slot = 1);

    Ray ScreenPointToRay(Vector3 screenPos);
    Vector3 WorldToScreenPoint(Vector3 worldPos);

    void SetTarget(Transform* target) { this->target = target; }
    void SetTargetTransform();

    void Shake(float magnitude, float duration);
    void Shaking();

    Matrix GetView() { return view; }
    Vector3* GetFocusOffset() { return &focusOffset; }

    void FreeMode();
    void FollowMode();
private:

    void SetView();

    void Save();
    void Load();
};