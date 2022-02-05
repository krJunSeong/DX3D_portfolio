#pragma once

class Zombie : public ModelAnimator
{
private:
    enum AnimState
    {
        IDLE, RUN, ATTACK, HIT, DEATH, JUMP_ATTACK
    }state;

    Matrix rightHand;

    Sword* sword;
    SphereCollider* collider;

    float hp = 100.0f;
    float lerpHp = 100.0f;
    float lerpSpeed = 1.0f;

    Bar* hpBar;

    Matrix rootTransform;
    Transform startTransform;

    bool playingRootMotion;

    LightBuffer::Light* light;
public:
    Zombie();
    ~Zombie();

    void Update();
    void Render();
    void PostRender();

    void GUIRender();

    void Damage();
private:
    void SetRightHand();
    void SetRoot();

    void SetClip(AnimState state);

    void EndAttack();
    void EndHit();
    void EndDeath();
    void EndJumpAttack();

    void EnableSword();
    void DisableSword();

    void LerpHp();
};
