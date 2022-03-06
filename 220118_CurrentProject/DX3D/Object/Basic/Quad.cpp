#include "Framework.h"

Quad::Quad(Float2 size) : size(size)
{
    material = new Material(L"Lighting.hlsl");

    CreateMesh();

    worldBuffer = new WorldBuffer();
    colorBuffer = new ColorBuffer();
}

Quad::~Quad()
{
    delete material;
    delete mesh;
    delete worldBuffer;
    delete colorBuffer;
}

void Quad::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);

    colorBuffer->SetPSBuffer(6);

    mesh->IASet();
    material->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::SetRender()
{
    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);

    mesh->IASet();
    material->Set();
}

void Quad::CreateMesh()
{
    float x = size.x * 0.5f;
    float y = size.y * 0.5f;

    vertices.resize(4);
    vertices[0].position = { -x, +y, 0.0f };
    vertices[1].position = { +x, +y, 0.0f };
    vertices[2].position = { -x, -y, 0.0f };
    vertices[3].position = { +x, -y, 0.0f };

    vertices[0].uv = { 0, 0 };
    vertices[1].uv = { 1, 0 };
    vertices[2].uv = { 0, 1 };
    vertices[3].uv = { 1, 1 };

    vertices[0].normal = { 0, 0, -1 };
    vertices[1].normal = { 0, 0, -1 };
    vertices[2].normal = { 0, 0, -1 };
    vertices[3].normal = { 0, 0, -1 };

    vertices[0].tangent = { 1, 0, 0 };
    vertices[1].tangent = { 1, 0, 0 };
    vertices[2].tangent = { 1, 0, 0 };
    vertices[3].tangent = { 1, 0, 0 };

    indices = { 0, 1, 2, 2, 1, 3 };

    mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
        indices.data(), indices.size());
}
