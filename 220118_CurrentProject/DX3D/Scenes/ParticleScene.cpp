#include "framework.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene()
{
	//particle = new Spark(L"Textures/Particle/Star.png", true);
    particle = new Sprite(L"Textures/Particle/fire_8x2.png", Float2(8, 2), true);
    //particle = new Rain();
    //particle = new Snow();

    particle->Play(Vector3(0,0,0));

	collider = new BoxCollider({ 50, 50, 1 });
}

ParticleScene::~ParticleScene()
{
	delete particle;
	delete collider;
}

void ParticleScene::Update()
{
    if (MOUSE_CLICK(0))
    {
        Contact contact;
        Ray ray = CAM->ScreenPointToRay(mousePos);
        if (collider->RayCollision(ray, &contact))
        {
            particle->Play(contact.hitPoint);
        }
    }

    particle->Update();
    collider->UpdateWorld();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
    particle->Render();
    collider->Render();
}

void ParticleScene::PostRender()
{
}

void ParticleScene::GUIRender()
{
    particle->GUIRender();
    collider->GUIRender();
}
