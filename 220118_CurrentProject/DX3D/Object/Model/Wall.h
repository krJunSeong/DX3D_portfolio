#pragma once

class Wall : public Model
{
private:

public:
    Wall();
    ~Wall();

    void Update();
    void Render();
    void GUIRender();
};