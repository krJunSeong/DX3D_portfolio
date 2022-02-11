#include "framework.h"

Snow::Snow()
{
    material->SetShader(L"Geometry/Snow.hlsl");
    material->SetDiffuseMap(L"Textures/Particle/Snow.png");

    geometryShader = Shader::AddGS(L"Geometry/Snow.hlsl");

    buffer = new WeatherBuffer();

    Create();
}

Snow::~Snow()
{
    delete buffer;
    delete[] vertices;
}

void Snow::Update()
{
    buffer->data.time += DELTA;
}

void Snow::Render()
{
    buffer->SetVSBuffer(10);
    Particle::Render();
}

void Snow::GUIRender()
{
    ImGui::Text("Snow Option");
    ImGui::DragFloat3("Velocity", (float*)&buffer->data.velocity, 0.1f);
    ImGui::SliderFloat("Distance", &buffer->data.distance, 0.0f, 500.0f);
    ImGui::DragFloat3("Origin", (float*)&buffer->data.origin, 0.1f);
    ImGui::DragFloat3("Size", (float*)&buffer->data.size, 0.1f);
    ImGui::SliderFloat("Tubulence", (float*)&buffer->data.turbulence, 0.0f, 10.0f);

    ImGui::ColorEdit4("Color", (float*)&buffer->data.color);
}

void Snow::Create()
{
    particleCount = MAX_COUNT;

    vertices = new VertexSnow[particleCount];

    for (UINT i = 0; i < particleCount; i++)
    {
        Float2 size;
        size.x = Random(0.5f, 2.0f);
        size.y = size.x;

        Float3 position;
        position.x = Random(-buffer->data.size.x, +buffer->data.size.x);
        position.y = Random(-buffer->data.size.y, +buffer->data.size.y);
        position.z = Random(-buffer->data.size.z, +buffer->data.size.z);

        vertices[i].position = position;
        vertices[i].size = size;

        vertices[i].random.x = Random(0.0f, 1.0f);
        vertices[i].random.y = Random(0.0f, 1.0f);
    }

    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSnow), particleCount);
}
