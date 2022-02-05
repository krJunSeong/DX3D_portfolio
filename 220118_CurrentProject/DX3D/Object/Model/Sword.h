#pragma once

class Sword : public Model
{
private:
    BoxCollider* collider;

public:
    Sword();
    ~Sword();

    void Update();
    void Render();

    BoxCollider* GetCollider() { return collider; }
};