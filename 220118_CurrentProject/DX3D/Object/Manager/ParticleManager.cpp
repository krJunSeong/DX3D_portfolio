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
			// 진행하지 않을 때는 넘긴다.
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
			// 진행하지 않을 때는 넘긴다.
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
	// 1번 인덱스로 관리
	if(num != 999)
	{
		particles[particleName][num]->Play(pos);
		return;
	}

	// 2번 pos 비교해서 관리하기
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