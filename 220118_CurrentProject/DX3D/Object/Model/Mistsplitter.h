#pragma once

class Mistsplitter : public Model
{
private:
    BoxCollider* collider;

public:
    Mistsplitter(Matrix* rightHand);
    ~Mistsplitter();

    void Update();
    void Render();

    BoxCollider* GetCollider() { return collider; }
};