#pragma once

class Amber : public ModelAnimator
{
private:
    enum AnimState
    {
        IDLE, RUN, ATTACK
    }state;

    Matrix rightHand;
    
    CapsuleCollider* collider;

    float hp = 100.0f;
    float lerpHp = 100.0f;
    float lerpSpeed = 1.0f;

    Bar* hpBar;
public:
    Amber();
    ~Amber();

    void Update();
    void Render();
    void PostRender();

    void GUIRender();

    void Damage();
private:
    void SetRightHand();

    void SetClip(AnimState state);

    void EndAttack();

    void LerpHp();
};
