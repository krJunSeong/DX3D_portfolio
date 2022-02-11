#include "framework.h"

Sprite::Sprite(wstring imageFile, Float2 maxFrame, bool isAdditive)
    : time(0.0f), speed(1.0f), curFrameCount(0), size(10, 10)
{
    material->SetShader(L"Geometry/Sprite.hlsl");
    material->SetDiffuseMap(imageFile);

    geometryShader = Shader::AddGS(L"Geometry/Sprite.hlsl");

    buffer = new SpriteBuffer();
    buffer->data.maxFrame = maxFrame;
    maxFrameCount = (UINT)(maxFrame.x * maxFrame.y);    // 4 x 4  = 16 °³

    if (isAdditive)
        blendState[1]->Additive();

    Create();
}

Sprite::~Sprite()
{
    delete buffer;
    delete vertices;
}

void Sprite::Update()
{
    if (!isActive) return;

    time += speed * DELTA;

    if (time > 0.1f)
    {
        curFrameCount++;
        buffer->data.curFrame.x = curFrameCount % (UINT)buffer->data.maxFrame.x;
        buffer->data.curFrame.y = curFrameCount / (UINT)buffer->data.maxFrame.x;
        time = 0.0f;
    }

    if (curFrameCount >= maxFrameCount)
        Stop();
}

void Sprite::Render()
{
    buffer->SetPSBuffer(10);

    Particle::Render();
}

void Sprite::GUIRender()
{
    ImGui::Text("Sprite Option");
    ImGui::SliderFloat("Speed", &speed, 0.1f, 10.0f);
    ImGui::SliderFloat2("Size", (float*)&size, 0.1f, 10.0f);
}

void Sprite::Play(Vector3 position)
{
    isActive = true;

    time = 0.0f;
    curFrameCount = 0;

    vertices->position = position;
    vertices->size = size;

    vertexBuffer->Update(vertices, particleCount);
}

void Sprite::Create()
{
    particleCount = 1;

    vertices = new VertexSize();
    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), particleCount);
}
