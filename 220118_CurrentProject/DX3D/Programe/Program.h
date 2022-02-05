#pragma once

class Program
{
public:
    Program();
    ~Program();

    void Update();

    void PreRender();
    void Render();
    void PostRender();
    void GUIRender();
private:
    void Create();
    void Delete();
};