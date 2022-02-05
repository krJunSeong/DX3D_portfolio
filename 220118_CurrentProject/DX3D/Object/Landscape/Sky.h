#pragma once

class Sky
{
private:
    Sphere* sphere;
    Texture* cubeMap;

    RasterizerState* rasterizerState[2];
    DepthStencilState* depthStencilState[2];

public:
    Sky(wstring file);
    ~Sky();

    void Render();
};