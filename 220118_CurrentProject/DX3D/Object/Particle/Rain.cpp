#include "framework.h"

Rain::Rain()
{
    material->SetShader(L"Geometry/Rain.hlsl");
    material->SetDiffuseMap(L"Textures/Particle/Rain.png");

    geometryShader = Shader::AddGS(L"Geometry/Rain.hlsl");

    buffer = new WeatherBuffer();

    Create();
}

Rain::~Rain()
{
    delete buffer;
    delete[] vertices;
}

void Rain::Update()
{
    buffer->data.time += DELTA;
}

void Rain::Render()
{
    buffer->SetVSBuffer(10);
    Particle::Render();
}

void Rain::GUIRender()
{
    ImGui::Text("Rain Option");
    ImGui::DragFloat3("Velocity", (float*)&buffer->data.velocity, 0.1f);
    ImGui::SliderFloat("Distance", &buffer->data.distance, 0.0f, 500.0f);
    ImGui::DragFloat3("Origin", (float*)&buffer->data.origin, 0.1f);
    ImGui::DragFloat3("Size", (float*)&buffer->data.size, 0.1f);
  
    ImGui::ColorEdit4("Color", (float*)&buffer->data.color);
}

void Rain::Create()
{
    particleCount = MAX_COUNT;

    vertices = new VertexSize[particleCount];

    for(UINT i = 0; i < particleCount; i++)
    {
        Float2 size;
        size.x = Random(0.1f, 0.4f);
        size.y = Random(4.0f, 8.0f);

        Float3 position;
        position.x = Random(-buffer->data.size.x, +buffer->data.size.x);
        position.y = Random(-buffer->data.size.y, +buffer->data.size.y);
        position.z = Random(-buffer->data.size.z, +buffer->data.size.z);

        vertices[i].position = position;
        vertices[i].size = size;
    }

    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), particleCount);
}