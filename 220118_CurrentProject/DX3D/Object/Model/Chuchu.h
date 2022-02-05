#pragma once

class Chuchu : public ModelAnimator
{
private:
    enum AnimState
    {
        IDLE, RUN, ATTACK, HIT, DEATH
    }state;

    Matrix rightHand;

    CapsuleCollider* collider;
    BoxCollider* attackCollider;

    float hp = 100.0f;
    float lerpHp = 100.0f;
    float lerpSpeed = 1.0f;
    float hpBarScaleRate = 20.0f;

    Quad* qLand;
    Terrain* tLand;

    Bar* hpBar;
public:
    Chuchu();
    ~Chuchu();

    void Update();
    void Render();
    void PostRender();

    void GUIRender();

    void Damage(float damage);
    
    Quad* GetQuad(){return qLand;}
    Terrain* GetTerrain() { return tLand; }
    void Init();

    void SetQuad(Quad* quad){ qLand = quad;}
    void SetTerrain(Terrain* terrain){tLand = terrain;}
private:
    void SetRightHand();

    void SetClip(AnimState state);

    void EndAttack();

    void LerpHp();
};