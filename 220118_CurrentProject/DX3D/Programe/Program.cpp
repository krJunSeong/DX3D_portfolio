#include "Framework.h"

#include "Scenes/TutorialScene.h"
#include "Scenes/CubeScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/TerrainScene.h"
#include "Scenes/SphereScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/ModelAnimatorScene.h"
#include "Scenes/CharacterScene.h"
#include "Scenes/CollisionScene.h"
#include "Scenes/LightScene.h"
#include "Scenes/ReportScene.h"
#include "Scenes/AstarScene.h"
#include "Scenes/FrustumScene.h"
#include "Scenes/InstancingScene.h"
#include "Scenes/ModelAnimatorInstancingScene.h"
#include "Scenes/DefenceScene.h"
#include "Scenes/BillboardScene.h"
#include "Scenes/ParticleScene.h"
#include "Scenes/QuadTreeScene.h"

Program::Program()
{
    Create();

    SceneManager::Get()->Create("Grid", new GridScene());
    SceneManager::Get()->Add("Grid");

    // �ͷ���
       //SceneManager::Get()->Create("Terrain", new TerrainEditorScene());
       //SceneManager::Get()->Add("Terrain");

    // Report Scene
       SceneManager::Get()->Create("Report", new ReportScene());
       SceneManager::Get()->Add("Report");
      
    // ��, �ִϸ��̼� �̴� ��, ModelData/FBX/  ���ϳֱ�
        //SceneManager::Get()->Create("Export", new ModelExportScene());
        //SceneManager::Get()->Create("ModelRender", new ModelRenderScene());
        //SceneManager::Get()->Add("ModelRender");
       
    // �ִϸ����� Test Scene 
        // "ModelData/Animations/��������"
        //SceneManager::Get()->Create("Animator", new ModelAnimatorScene());
        //SceneManager::Get()->Add("Animator");

    // ĳ����
        //SceneManager::Get()->Create("Character", new CharacterScene());
        //SceneManager::Get()->Add("Character");

    // �浹ó��
        //SceneManager::Get()->Create("Coliision", new CollisionScene());
        //SceneManager::Get()->Add("Coliision");

    // LightScene
        //SceneManager::Get()->Create("Light", new LightScene());
        //SceneManager::Get()->Add("Light");

    // Astar
        //SceneManager::Get()->Create("Astar", new AstarScene());
        //SceneManager::Get()->Add("Astar");
     

    // Frustum Curing
        //SceneManager::Get()->Create("Frustum", new FrustumScene());
        //SceneManager::Get()->Add("Frustum");

    // Instancing
        //SceneManager::Get()->Create("Instance", new InstancingScene());
        //SceneManager::Get()->Add("Instance");
    
    // ModelAnimatorInstancingScene
        //SceneManager::Get()->Create("ModelAnimatorInstancingScene", new ModelAnimatorInstancingScene());
        //SceneManager::Get()->Add("ModelAnimatorInstancingScene");

    // ������ Tree ��
    //SceneManager::Get()->Create("BillboardScene", new BillboardScene());
    //SceneManager::Get()->Add("BillboardScene");

    // Particle Scene
    //SceneManager::Get()->Create("ParticleScene", new ParticleScene());
    //SceneManager::Get()->Add("ParticleScene");

}
Program::~Program()
{
    Delete();
}

void Program::Update()
{
    SceneManager::Get()->Update();

    CAM->Update();
    FRUSTUM->Update();
}

void Program::PreRender()
{
    SceneManager::Get()->PreRender();
}

void Program::Render()
{
    Device::Get()->SetRenderTarget();
    Environment::Get()->Set();

    SceneManager::Get()->Render();
}

void Program::PostRender()
{
    Environment::Get()->PostSet();

    Font::Get()->GetDC()->BeginDraw();

    string fps = "FPS : " + to_string((int)ImGui::GetIO().Framerate);
    Font::Get()->RenderText(fps, "default", Float2(50, 10));

    SceneManager::Get()->PostRender();

    Font::Get()->GetDC()->EndDraw();
}

void Program::GUIRender()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static bool isActive = true;

    if (isActive)
    {
        ImGui::Begin("Inspector", &isActive);
        Environment::Get()->SetImGui();
        SceneManager::Get()->GUIRender();
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Program::Create()
{
    Device::Get();
    Font::Get();
    Environment::Get();
    SceneManager::Get();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);

    Font::Get()->Add("default", L"���� ���");
}

void Program::Delete()
{
    Device::Delete();
    Shader::Delete();
    Texture::Delete();
    Font::Delete();
    Environment::Delete();
    SceneManager::Delete();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
