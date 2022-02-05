#include "Framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
    terrain = new TerrainEditor();

    ByteAddress();
}

TerrainEditorScene::~TerrainEditorScene()
{
    delete terrain;
}

void TerrainEditorScene::Update()
{
    /*if (MOUSE_CLICK(0))
    {
        pickingPos = terrain->Picking();
        ReportRobot_211201* robot = CreateRobot(pickingPos);
        robots.push_back(robot);
    }*/

   // pickingPos = terrain->ComputePicking();

    for (auto robot : robots)
        robot->Update();

    terrain->Update();
}

void TerrainEditorScene::PreRender()
{
}

void TerrainEditorScene::Render()
{
    terrain->Render();
    for (auto robot : robots)
        robot->Render();
}

void TerrainEditorScene::PostRender()
{
}

void TerrainEditorScene::GUIRender()
{
    terrain->GUIRender();
}

void TerrainEditorScene::ByteAddress()
{
    ComputeShader* shader = Shader::AddCS(L"ByteAddress.hlsl");

    struct Output
    {
        UINT groupID[3];
        UINT groupThreadID[3];
        UINT dispathThreadID[3];
        UINT groupIndex;
    };

    UINT groupX = 2;
    UINT size = 10 * 8 * 3 * groupX;// 쓰레드 개수

    Output* output = new Output[size];
    RawBuffer* buffer = new RawBuffer(sizeof(Output) * size);

    DC->CSSetUnorderedAccessViews(0, 1, &buffer->GetUAV(), nullptr);

    shader->Set();

    DC->Dispatch(groupX, 1, 1);

    buffer->Copy(output, sizeof(Output)* size);

    FILE* file;
    fopen_s(&file, "TextData/RawTest.csv", "w");

    for (UINT i = 0; i < size; i++)
    {
        Output temp = output[i];

        fprintf
        (
            file,
            "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
            i,
            temp.groupID[0], temp.groupID[1], temp.groupID[2],
            temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
            temp.dispathThreadID[0], temp.dispathThreadID[1], temp.dispathThreadID[2],
            temp.groupIndex
        );
    }

    fclose(file);
}

ReportRobot_211201* TerrainEditorScene::CreateRobot(Float3 pos)
{
    ReportRobot_211201* robot = new ReportRobot_211201(pos);

    return robot;
}
