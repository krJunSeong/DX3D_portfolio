#include "Framework.h"

Grid::Grid()
    : width(MAX_VALUE), height(MAX_VALUE)
{
    material = new Material(L"Grid.hlsl");

    CreateMesh();

    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
    worldBuffer = new WorldBuffer();
}

Grid::~Grid()
{
    delete material;
    delete vertexBuffer;

    delete worldBuffer;
}

void Grid::Render()
{
    worldBuffer->SetVSBuffer(0);
    vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    material->Set();

    DC->Draw(vertices.size(), 0);
}

void Grid::SetSize()
{
    POINT size = { width, height };

    ImGui::InputInt2("GridSize", (int*)&size);

    if (width == size.x && height == size.y)
        return;

    width = size.x;
    height = size.y;

    CreateMesh();
    vertexBuffer->Update(vertices.data(), vertices.size());
}

void Grid::CreateMesh()
{
    int halfW = width / 2;
    int halfH = height / 2;

    vertices.clear();

    for (int x = -halfW; x <= halfW; x++)
    {
        if (x == 0) continue;

        vertices.emplace_back(x, 0, -halfH, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(x, 0, +halfH, 0.5f, 0.5f, 0.5f);
    }

    for (int z = -halfH; z <= halfH; z++)
    {
        if (z == 0) continue;

        vertices.emplace_back(-halfW, 0, z, 0.5f, 0.5f, 0.5f);
        vertices.emplace_back(+halfW, 0, z, 0.5f, 0.5f, 0.5f);
    }

    //Axis X
    vertices.emplace_back(-MAX_VALUE, 0, 0, 1, 0, 0);
    vertices.emplace_back(+MAX_VALUE, 0, 0, 1, 0, 0);

    //Axis Y
    vertices.emplace_back(0, -MAX_VALUE, 0, 0, 1, 0);
    vertices.emplace_back(0, +MAX_VALUE, 0, 0, 1, 0);

    //Axis Z
    vertices.emplace_back(0, 0, -MAX_VALUE, 0, 0, 1);
    vertices.emplace_back(0, 0, +MAX_VALUE, 0, 0, 1);
}
