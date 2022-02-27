#include "Framework.h"

Environment::Environment()
{
    SetViewport();
    CreateProjection();

    uiViewBuffer = new MatrixBuffer();
    lightBuffer = new LightBuffer();

    samplerState = new SamplerState();
    samplerState->SetState(0);

    camera = new Camera();
    frustum = new Frustum(projection);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Alpha(true);

    depthState[0] = new DepthStencilState();
    depthState[1] = new DepthStencilState();
    depthState[1]->DepthEnable(false);
}

Environment::~Environment()
{
    delete projectionBuffer;
    delete lightBuffer;

    delete camera;
    delete frustum;

    delete uiViewBuffer;
    delete orthoBuffer;

    delete blendState[0];
    delete blendState[1];

    delete depthState[0];
    delete depthState[1];
}

void Environment::Set()
{
    DC->RSSetViewports(1, &viewport);

    camera->SetBuffer();
    projectionBuffer->SetVSBuffer(2);
    projectionBuffer->SetGSBuffer(2);

    lightBuffer->SetPSBuffer(0);

    blendState[0]->SetState();
    depthState[0]->SetState();
}

void Environment::PostSet()
{
    uiViewBuffer->SetVSBuffer(1);
    orthoBuffer->SetVSBuffer(2);

    blendState[1]->SetState();
    depthState[1]->SetState();
}

void Environment::SetImGui()
{
    camera->GUIRender();

    if (ImGui::TreeNode("LightOption"))
    {
        if (ImGui::Button("Add"))
            lightBuffer->data.lightCount++;
        ImGui::SameLine();
        if (ImGui::Button("Remove"))
            lightBuffer->data.lightCount--;
        ImGui::SameLine();
        if (ImGui::Button("Save")) Save();
        ImGui::SameLine();
        if (ImGui::Button("Load")) Load();

        for (UINT i = 0; i < lightBuffer->data.lightCount; i++)
        {
            SetLight(i);
        }

        ImGui::ColorEdit3("AmbientLight", (float*)&lightBuffer->data.ambient);
        ImGui::ColorEdit3("AmbientCeil", (float*)&lightBuffer->data.ambientCeil);
        ImGui::TreePop();
    }

}

LightBuffer::Light* Environment::AddLight()
{
    int index = lightBuffer->data.lightCount++;

    return &lightBuffer->data.lights[index];
}

void Environment::SetViewport()
{
    viewport.Width = WIN_WIDTH;
    viewport.Height = WIN_HEIGHT;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
}

void Environment::CreateProjection()
{
    projectionBuffer = new MatrixBuffer();

    projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 1000.0f);

    projectionBuffer->Set(projection);

    orthoBuffer = new MatrixBuffer();

    Matrix ortho = XMMatrixOrthographicOffCenterLH(0.0f, WIN_WIDTH, 0.0f, WIN_HEIGHT, -1.0f, 1.0f);

    orthoBuffer->Set(ortho);
}

void Environment::SetLight(UINT index)
{
    string name = "Light_" + to_string(index);
    LightBuffer::Light& light = lightBuffer->data.lights[index];

    if (ImGui::TreeNode(name.c_str()))
    {
        ImGui::Checkbox("Active", (bool*)&light.active);

        const char* list[] = { "Directional", "Point", "Spot", "Capsule" };
        ImGui::Combo("Type", (int*)&light.type, list, 4);

        ImGui::ColorEdit3("Color", (float*)&light.color, ImGuiColorEditFlags_PickerHueWheel);
        ImGui::SliderFloat3("Dir", (float*)&light.direction, -1.0f, 1.0f);
        ImGui::DragFloat3("Pos", (float*)&light.position);
        ImGui::SliderFloat("Range", &light.range, 0, 500);
        ImGui::SliderFloat("Inner", &light.inner, 0, light.outer);
        ImGui::SliderFloat("Outer", &light.outer, light.inner, 180);
        ImGui::DragFloat("Length", &light.length);

        ImGui::TreePop();
    }
}

void Environment::Save(string name)
{
    BinaryWriter w("TextData/OptionData/" + name);

    int lightCount = lightBuffer->data.lightCount;

    // 라이트의 개수
    w.Int(lightCount);
        
    // for문으로 그 수만큼 돌리셈
    for(int i = 0; i < lightCount; i++)
    {
        w.Int(lightBuffer->data.lights[i].active);
        w.Int((int)lightBuffer->data.lights[i].type);

        w.Float(lightBuffer->data.lights[i].color.x);
        w.Float(lightBuffer->data.lights[i].color.y);
        w.Float(lightBuffer->data.lights[i].color.z);
        w.Float(lightBuffer->data.lights[i].color.w);

        w.Float(lightBuffer->data.lights[i].direction.x);
        w.Float(lightBuffer->data.lights[i].direction.y);
        w.Float(lightBuffer->data.lights[i].direction.z);

        w.Vector(lightBuffer->data.lights[i].position);
        
        w.Float(lightBuffer->data.lights[i].range);
        
        w.Float(lightBuffer->data.lights[i].inner);
        
        w.Float(lightBuffer->data.lights[i].outer);

        w.Float(lightBuffer->data.lights[i].length);
    }
}

void Environment::Load(string name)
{
    BinaryReader r("TextData/OptionData/" + name);

    if (r.IsFailed()) return;

    // 라이트의 개수
    lightBuffer->data.lightCount = r.Int();
    int lightCount = lightBuffer->data.lightCount;

    // for문으로 그 수만큼 돌리셈
    for (int i = 0; i < lightCount; i++)
    {
        lightBuffer->data.lights[i].active = r.Int();
        lightBuffer->data.lights[i].type = (LightType)r.Int();

        lightBuffer->data.lights[i].color.x = r.Float();
        lightBuffer->data.lights[i].color.y = r.Float();
        lightBuffer->data.lights[i].color.z = r.Float();
        lightBuffer->data.lights[i].color.w = r.Float();
        
        lightBuffer->data.lights[i].direction.x = r.Float();
        lightBuffer->data.lights[i].direction.y = r.Float();
        lightBuffer->data.lights[i].direction.z = r.Float();
        
        lightBuffer->data.lights[i].position = r.Vector();

        lightBuffer->data.lights[i].range = r.Float();
        
        lightBuffer->data.lights[i].inner = r.Float();
        
        lightBuffer->data.lights[i].outer = r.Float();
        
        lightBuffer->data.lights[i].length = r.Float();
    }
}
