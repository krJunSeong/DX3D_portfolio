#include "framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()
{
   /*
    int dan = 10;
    float size = 0.15f;
    for(int y = 0; y < dan; y++)
    {
        for(int x = 0; x < dan - y; x++)
        {
            for(int z = 0; z < dan - y; z++)
            {
                Cube* tempCube = new Cube(Float3((x - (dan - y) * 0.5f) * size, // 
                                                  y * size,
                                                (z - (dan - y) * 0.5f) * size));
                cubes.push_back(tempCube);
            }
        }
    }*/

    ReportCube* tempCube = new ReportCube(Float3(0, 0, 0));
    cubes.push_back(tempCube);

   // worldBuffer = new MatrixBuffer();
    viewBuffer = new MatrixBuffer();
    projectionBuffer = new MatrixBuffer();

    Vector4 eye = XMVectorSet(3, 3, -3, 0);//카메라 위치
    Vector4 focus = XMVectorSet(0, 0, 0, 0);
    Vector4 up = XMVectorSet(0, 1, 0, 0);

    Matrix view = XMMatrixLookAtLH(eye, focus, up);
    viewBuffer->Set(view);

    Matrix projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);
    projectionBuffer->Set(projection);
    

    //texture = Texture::Add("Textures/Landscape/Box.png");
}

TutorialScene::~TutorialScene()
{
    for(auto c : cubes)
        delete c;

    delete worldBuffer;
    delete viewBuffer;
    delete projectionBuffer;
}

void TutorialScene::Update()
{
   //static float angle = 0.0f;
   //angle += 0.005f;
   //Matrix world = XMMatrixRotationY(angle);
   
   //worldBuffer->Set(world);

    for (auto c : cubes) c->Update();
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
    for (auto c : cubes) c->Render();

   // worldBuffer->SetVSBuffer(0);
    viewBuffer->SetVSBuffer(1);
    projectionBuffer->SetVSBuffer(2);
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
    if(ImGui::Button("test"))
        ImGuiFileDialog::Instance()->OpenDialog("Test", "test", ".*", ".");

    if(ImGuiFileDialog::Instance()->Display("Test"))
    {
        ImGuiFileDialog::Instance()->Close();
    }
}