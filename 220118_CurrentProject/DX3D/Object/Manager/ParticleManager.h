#pragma once

class ParticleManager : public Singleton<ParticleManager>
{
private:
    map<string, vector<ParticleSystem*>> particles;

public:
    ParticleManager();
    ~ParticleManager();

    void Update();
    void Render();
    void GUIRender();

    void CreateParticle(string name = "");

    void Play(string particleName, Vector3 pos, int num = 999);
    void Stop(string particleName, Vector3 pos, int num = 999);
};