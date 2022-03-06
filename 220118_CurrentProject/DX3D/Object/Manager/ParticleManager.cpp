#include "framework.h"

ParticleManager::ParticleManager()
{

}

ParticleManager::~ParticleManager()
{
	for(auto& mapParticle : particles)
	{
		for(ParticleSystem* particle : mapParticle.second)
			delete particle;
	}
}

void ParticleManager::Update()
{
	for (auto& mapParticle : particles)
	{
		for (ParticleSystem* particle : mapParticle.second)
		{
			// �������� ���� ���� �ѱ��.
			if(!particle->GetIsActive()) continue;

			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto& mapParticle : particles)
	{
		for (ParticleSystem* particle : mapParticle.second)
		{
			// �������� ���� ���� �ѱ��.
			if (!particle->GetIsActive()) continue;

			particle->Render();
		}
	}
}

void ParticleManager::GUIRender()
{
}


void ParticleManager::Play(string particleName, Vector3 pos, int num)
{
	// 1�� �ε����� ����
	if(num != 999)
	{
		particles[particleName][num]->Play(pos);
		return;
	}

	// 2�� pos ���ؼ� �����ϱ�
	for(ParticleSystem* particle : particles[particleName])
	{
		if(particle->GetIsActive()) continue;

		particle->Play(pos);
		return;
	}
}
void ParticleManager::Stop(string particleName, Vector3 pos, int num)
{
	if(num != 999)
		particles[particleName][num]->Stop();

	for (ParticleSystem* particle : particles[particleName])
	{
		if (!particle->GetIsActive()) continue;

		Vector3 particlePos = particle->GetQuadPosition();

		bool ch = false;

		if(particlePos == pos) ch = true;

		particle->Stop();

		return;
	}
}

void ParticleManager::CreateParticle(string name)
{
	vector<ParticleSystem*> temp;

	for(int i = 0; i < 5; i++)
	{
		ParticleSystem* tempPs = new ParticleSystem();
		tempPs->Load("TextData/Particles/UpStar.fx");

		temp.push_back(tempPs);
	}

	particles["UpStar"] = temp;

	temp.clear();

	// -------------------------------------------- Create HorizonStar --------------------------------------------
	ParticleSystem* tempPs = new ParticleSystem();
	tempPs->Load("TextData/Particles/HorizonStar.fx");
	temp.push_back(tempPs);

	particles["HorizonStar"] = temp;
}