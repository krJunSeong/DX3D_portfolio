#pragma once

class TerrainEditorScene : public Scene
{
private:
    TerrainEditor* terrain;

    vector<ReportRobot_211201*> robots;

    bool isCreate = false;


public:
    TerrainEditorScene();
    ~TerrainEditorScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void ByteAddress();
    ReportRobot_211201* CreateRobot(Float3 pos);
};