#include "Framework.h"
#include "TerrainEditorScene.h"

TerrainEditorScene::TerrainEditorScene()
{
    terrain = new TerrainEditor(200, 200);
    //player = new Jean();
    ByteAdrress();
}

TerrainEditorScene::~TerrainEditorScene()
{
    delete terrain;
    //delete player;
}

void TerrainEditorScene::Update()
{
    terrain->Update();
    //player->Update();
}

void TerrainEditorScene::PreRender()
{
}

void TerrainEditorScene::Render()
{
    terrain->Render();
    //player->Render();
}

void TerrainEditorScene::PostRender()
{
}

void TerrainEditorScene::GUIRender()
{
    terrain->GUIRender();
}

void TerrainEditorScene::ByteAdrress()
{
    ComputeShader* shader = Shader::AddCS(L"Old/ByteAddress.hlsl");

    struct Output
    {
        UINT groupID[3];
        UINT groupThreadID[3];
        UINT dispathThreadID[3];
        UINT groupIndex;
    };

    UINT groupX = 2;
    UINT size = 10 * 8 * 3 * groupX;

    Output* output = new Output[size];
    RawBuffer* buffer = new RawBuffer(sizeof(Output) * size);

    DC->CSSetUnorderedAccessViews(0, 1, &buffer->GetUAV(), nullptr);

    shader->Set();

    DC->Dispatch(groupX, 1, 1);

    buffer->Copy(output, sizeof(Output) * size);

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
