#pragma once

class Spark : public Particle
{
private:
    struct VertexParticle
    {
        Float3 position = { 0, 0, 0 };
        Float2 size = { 1, 1 };
        Float3 velocity = { 0, 0, 0 };
    };

    ValueBuffer* sparkBuffer;
    ColorBuffer* startColorBuffer;
    ColorBuffer* endColorBuffer;

    VertexParticle* vertices;

    float minRadius = 5.0f;
    float maxRadius = 10.0f;

    float minSize = 0.1f;
    float maxSize = 1.0f;

public:
    Spark(wstring imageFile, bool isAdditive);
    ~Spark();

    void Update();
    void Render();
    void GUIRender();

    void Play(Vector3 position);

private:
    void Create();
    void UpdateParticle();
};