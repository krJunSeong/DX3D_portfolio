#include "framework.h"
/*
   1   3  --------    5   7
   
   0   2  --------    4   6
*/ 
ReportCube::ReportCube(Float3 pos)
{
    texture = Texture::Add(L"Textures/Landscape/Box.png");

    vertexShader = Shader::AddVS(L"UvShader.hlsl");
    pixelShader = Shader::AddPS(L"UvShader.hlsl");

    // 사각형 좌표 position 3, color 3
    /* vertices가 VertexColor일 때 쓴 것
        vertices.emplace_back(pos.x + -0.1f, pos.y - 0.1f, pos.z - 0.1f, 1, 0, 0);//0
        vertices.emplace_back(pos.x + -0.1f, pos.y + 0.1f, pos.z - 0.1f, 0, 1, 0);//1
        vertices.emplace_back(pos.x + +0.1f, pos.y - 0.1f, pos.z - 0.1f, 0, 0, 1);//2
        vertices.emplace_back(pos.x + +0.1f, pos.y + 0.1f, pos.z - 0.1f, 1, 1, 0);//3

        vertices.emplace_back(pos.x + -0.1f, pos.y + -0.1f, pos.z + 0.1f, 1, 0, 1);//4
        vertices.emplace_back(pos.x + -0.1f, pos.y + +0.1f, pos.z + 0.1f, 0, 1, 1);//5
        vertices.emplace_back(pos.x + +0.1f, pos.y + -0.1f, pos.z + 0.1f, 0, 0, 0);//6
        vertices.emplace_back(pos.x + +0.1f, pos.y + +0.1f, pos.z + 0.1f, 1, 1, 1);//7

         indices = {
        // Front
        0, 1, 2, 2, 1, 3,
        // Top
        1, 5, 3, 3, 5, 7,
        // Right
        2, 3, 7, 2, 7, 6,
        // Left   // 반시계방향, 정방향에서 보이는 게 시계방향이니ㅏ까.
        5, 1, 0, 0, 4, 5,
        // bottom
        0, 2, 4, 2, 6, 4,
        // back
        4, 6, 5, 6, 7, 5
        };
    */
    
    // Front
    vertices.emplace_back(-0.5, -0.5, -0.5, 0, 0);
    vertices.emplace_back(-0.5, +0.5, -0.5, 1, 0);
    vertices.emplace_back(+0.5, -0.5, -0.5, 0, 1);
    vertices.emplace_back(+0.5, +0.5, -0.5, 1, 1);
    // Top                 0.5   0.5   0.5
    vertices.emplace_back(-0.5, +0.5, -0.5, 0, 0);
    vertices.emplace_back(-0.5, +0.5, +0.5, 1, 0);
    vertices.emplace_back(+0.5, +0.5, -0.5, 0, 1);
    vertices.emplace_back(+0.5, +0.5, +0.5, 1, 1);
    // Right               0.5   0.5   0.5
    vertices.emplace_back(+0.5, -0.5, -0.5, 0, 0);
    vertices.emplace_back(+0.5, +0.5, -0.5, 1, 0);
    vertices.emplace_back(+0.5, -0.5, +0.5, 0, 1);
    vertices.emplace_back(+0.5, +0.5, +0.5, 1, 1);
    // Left                0.5   0.5   0.5
    vertices.emplace_back(-0.5, -0.5, +0.5, 0, 0);
    vertices.emplace_back(-0.5, +0.5, +0.5, 1, 0);
    vertices.emplace_back(-0.5, -0.5, -0.5, 0, 1);
    vertices.emplace_back(-0.5, +0.5, -0.5, 1, 1);
    // bottom              0.5   0.5   0.5
    vertices.emplace_back(-0.5, -0.5, -0.5, 0, 1);
    vertices.emplace_back(+0.5, -0.5, -0.5, 1, 1);
    vertices.emplace_back(-0.5, -0.5, +0.5, 0, 0);
    vertices.emplace_back(+0.5, -0.5, +0.5, 1, 0);
    // back                0.5   0.5   0.5
    vertices.emplace_back(+0.5, -0.5, +0.5, 1, 1);
    vertices.emplace_back(+0.5, +0.5, +0.5, 0, 1);
    vertices.emplace_back(-0.5, -0.5, +0.5, 1, 0);
    vertices.emplace_back(-0.5, +0.5, +0.5, 0, 0);
    /*
    vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0, 0);
    vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0, 1);
    vertices.emplace_back(+0.5f, -0.5f, -0.5f, 1, 0);
    vertices.emplace_back(+0.5f, +0.5f, -0.5f, 1, 1);

    vertices.emplace_back(-0.5f, -0.5f, +0.5f, 0, 1);//4
    vertices.emplace_back(-0.5f, +0.5f, +0.5f, 0, 0);//5
    vertices.emplace_back(+0.5f, -0.5f, +0.5f, 1, 1);//6
    vertices.emplace_back(+0.5f, +0.5f, +0.5f, 1, 0);//7
    */
    // sizeof(Vertex) -> sizeof(VertexColor)
    vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), vertices.size());

    indices = {
        //front
        0, 1, 2, 2, 1, 3,
        // Top
        4, 5, 6, 6, 5, 7,
        // Right
        8, 9, 10, 10, 9, 11,
        // Left
        12, 13, 14, 14, 13, 15,
        // bottom
        16, 17, 18, 18, 17, 19,
        // back
        20, 21, 22, 22, 21, 23
  };

    indexBuffer = new IndexBuffer(indices.data(), indices.size());

    worldBuffer = new MatrixBuffer();
}

ReportCube::ReportCube(Float3 pos, float r, Float3 color)
{
}

ReportCube::~ReportCube()
{
    delete vertexBuffer;
    delete indexBuffer;
    
    delete worldBuffer;
}

void ReportCube::Update()
{
    static float angle = 0.0f;
    angle += 0.0005f;
    Matrix world = XMMatrixRotationRollPitchYaw(angle,angle,0.0f);
    
    worldBuffer->Set(world);
}

void ReportCube::Render()
{
    worldBuffer->SetVSBuffer(0);
    vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    indexBuffer->IASet();

    texture->Set(0);

    vertexShader->Set();
    pixelShader->Set();

    DC->DrawIndexed(indices.size(), 0, 0);
}
