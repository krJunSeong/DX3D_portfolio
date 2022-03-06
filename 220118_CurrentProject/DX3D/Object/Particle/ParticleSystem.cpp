#include "Framework.h"

ParticleSystem::ParticleSystem(wstring path)
{
    quad = new Quad();
    quad->GetMaterial()->SetShader(L"ParticleSystem.hlsl");
    quad->GetMaterial()->SetDiffuseMap(path);

    datas.resize(MAX_COUNT);
    instanceData.resize(MAX_COUNT);

    instanceBuffer = new VertexBuffer(instanceData.data(), sizeof(InstanceData), MAX_COUNT);

    blendState[0] = new BlendState();
    blendState[1] = new BlendState();
    blendState[1]->Alpha(true);

    depthState[0] = new DepthStencilState();
    depthState[1] = new DepthStencilState();
    depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

ParticleSystem::~ParticleSystem()
{
    delete quad;
    delete instanceBuffer;

    delete blendState[0];
    delete blendState[1];

    delete depthState[0];
    delete depthState[1];
}

void ParticleSystem::Update()
{
    if (!quad->isActive) return;

    time += DELTA;

    UpdatePhysical();
    UpdateColor();
    quad->UpdateWorld();

    if (time > particleData.duration)
    {
        if (particleData.isLoop)
            Init();
        else
            Stop();
    }
}

void ParticleSystem::Render()
{
    if (!quad->isActive) return;

    instanceBuffer->IASet(1);

    quad->SetRender();

    blendState[1]->SetState();
    depthState[1]->SetState();

    DC->DrawIndexedInstanced(6, drawCount, 0, 0, 0);

    blendState[0]->SetState();
    depthState[0]->SetState();
}

void ParticleSystem::GUIRender()
{
    ImGui::Text("Particle Option");
    ImGui::Checkbox("Loop", &particleData.isLoop);
    ImGui::SliderInt("Count", (int*)&particleData.count, 0, MAX_COUNT);
    ImGui::SliderFloat("Duration", &particleData.duration, 0, 30.0f);
    ImGui::DragFloat3("MinVelocity", (float*)&particleData.minVelocity, 0.1f);
    ImGui::DragFloat3("MaxVelocity", (float*)&particleData.maxVelocity, 0.1f);
    ImGui::DragFloat3("MinAccelation", (float*)&particleData.minAccelation, 0.1f);
    ImGui::DragFloat3("MaxAccelation", (float*)&particleData.maxAccelation, 0.1f);
    ImGui::DragFloat3("MinScale", (float*)&particleData.minScale, 0.1f);
    ImGui::DragFloat3("MaxScale", (float*)&particleData.maxScale, 0.1f);
    ImGui::SliderFloat("MinStartTime", &particleData.minStartTime, 0, particleData.duration);
    ImGui::SliderFloat("MaxStartTime", &particleData.maxStartTime, 0, particleData.duration);
    ImGui::SliderFloat("MinSpeed", &particleData.minSpeed, 0, 10.0f);
    ImGui::SliderFloat("MaxSpeed", &particleData.maxSpeed, 0, 10.0f);
    ImGui::SliderFloat("MinAngularVelocity", &particleData.minAngularVelocity, -10.0f, 10.0f);
    ImGui::SliderFloat("MaxAngularVelocity", &particleData.maxAngularVelocity, -10.0f, 10.0f);
    ImGui::SliderFloat("WorldRotationY", &particleData.worldRotationY, 0, XM_2PI);

    ImGui::ColorEdit4("StartColor", (float*)&particleData.startColor);
    ImGui::ColorEdit4("EndColor", (float*)&particleData.endColor);

    quad->GetMaterial()->GUIRender();
}

void ParticleSystem::Play(Vector3 pos, Vector3 rot)
{
    quad->isActive = true;
    quad->position = pos;
    quad->rotation = rot;

    Init();
}

void ParticleSystem::Stop()
{
    quad->isActive = false;
}

void ParticleSystem::Save(string file)
{
    CreateFolders(file);

    BinaryWriter w(file);

    wstring diffuseFile = quad->GetMaterial()->GetDiffuseMap()->GetFile();
    w.WString(diffuseFile);

    w.Byte(&particleData, sizeof(ParticleData));
}

void ParticleSystem::Load(string file)
{
    BinaryReader r(file);

    wstring diffuseFile = r.WString();
    quad->GetMaterial()->SetDiffuseMap(diffuseFile);

    void* data = &particleData;

    r.Byte(&data, sizeof(ParticleData));
}

void ParticleSystem::UpdatePhysical()
{
    drawCount = 0;

    for (UINT i = 0; i < particleData.count; i++)
    {
        if (datas[i].startTime >= time) continue;

        datas[i].velocity += datas[i].accelation * DELTA;
        datas[i].transform.position += datas[i].velocity * datas[i].speed * DELTA;
        datas[i].transform.rotation.z += datas[i].angularVelocity * DELTA;
        if (particleData.isBillboard)
        {
            datas[i].transform.rotation.x = CAM->rotation.x;
            datas[i].transform.rotation.y = CAM->rotation.y - quad->rotation.y;
        }
        datas[i].transform.UpdateWorld();
        instanceData[drawCount].transform = XMMatrixTranspose(datas[i].transform.GetWorld());

        drawCount++;
    }

    quad->rotation.y += particleData.worldRotationY * DELTA;

    instanceBuffer->Update(instanceData.data(), drawCount);
}

void ParticleSystem::UpdateColor()
{
    float t = time / particleData.duration;

    Float4 color;
    color.x = LERP(particleData.startColor.x, particleData.endColor.x, t);
    color.y = LERP(particleData.startColor.y, particleData.endColor.y, t);
    color.z = LERP(particleData.startColor.z, particleData.endColor.z, t);
    color.w = LERP(particleData.startColor.w, particleData.endColor.w, t);

    quad->GetMaterial()->GetData().diffuse = color;
}

void ParticleSystem::Init()
{
    if (particleData.isAdditive)
    {
        blendState[1]->Additive();
    }
    else
    {
        blendState[1]->Alpha(true);
    }

    time = 0.0f;
    drawCount = 0;

    for (UINT i = 0; i < particleData.count; i++)
    {
        datas[i].transform = Transform();
        datas[i].transform.scale = Random(particleData.minScale, particleData.maxScale);

        datas[i].velocity = Random(particleData.minVelocity, particleData.maxVelocity);
        datas[i].velocity.Normalize();
        datas[i].accelation = Random(particleData.minAccelation, particleData.maxAccelation);
        datas[i].startTime = Random(particleData.minStartTime, particleData.maxStartTime);
        datas[i].speed = Random(particleData.minSpeed, particleData.maxSpeed);
        datas[i].angularVelocity = Random(particleData.minAngularVelocity, particleData.maxAngularVelocity);
    }
}

