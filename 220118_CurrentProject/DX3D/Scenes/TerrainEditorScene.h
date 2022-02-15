#pragma once

class TerrainEditorScene : public Scene
{
private:
    TerrainEditor* terrain;
    //Jean* player;
public:
    TerrainEditorScene();
    ~TerrainEditorScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void ByteAdrress();
};