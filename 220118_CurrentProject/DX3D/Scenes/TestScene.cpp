#include "framework.h"
#include "TestScene.h"

TestScene::TestScene()
{
	ui = new UIPanel();
	ui->tag = "panel";

	button = new Button(L"Textures/UI/selected.png");

	hpBar = new Bar(L"Textures/UI/ProgressBar_Fill_Health.png", L"Textures/UI/ProgressBar2_Background.png", 200.0f);
	hpBar->tag = "hp";
	hpBar->position = { 280, 75, 0 };

	staminaBar = new Bar(L"Textures/UI/stamina_bar1.png", L"Textures/UI/ProgressBar2_Background.png", stamina);
	staminaBar->tag = "stamina";
	staminaBar->position = { 280, 75, 0 };
	staminaBar->Load();

	icon = new Quad();
	icon->tag = "icon";
	icon->GetMaterial()->SetDiffuseMap(L"Textures/UI/Icon.png");
	icon->GetMaterial()->SetShader(L"Texture.hlsl");
	icon->Load();

	particleSystem = new ParticleSystem();
	particleSystem->Load("TextData/Particles/Test.fx");
	particleSystem->Play({0,0,0});

	particle = new Sprite(L"Textures/Particle/fire_8x2.png", Float2(8, 2), true);
	
	ParticleManager::Get();

	//bar->scale *= 1.0f;
	// 경로 버그
	//quad->GetMaterial()->SetDiffuseMap(L"Textures/UI/UnitFrame_Health_Bar.png");
	//quad->GetMaterial()->SetShader(L"Texture.hlsl");
}

TestScene::~TestScene()
{
	delete ui;
	delete button;
	delete hpBar;
	delete icon;
	delete staminaBar;
	delete particleSystem;
	delete particle;
}

void TestScene::Update()
{
	ui->Update();
	button->Update();

	if(KEY_DOWN(VK_SPACE)) 
	{
		hp -= 10;
		stamina -= 10;
	}

	hp += DELTA;
	stamina += DELTA;

	lerpHp = LERP(lerpHp, hp, lerpSpeed * DELTA);	
	lerpStamina = LERP(lerpStamina, stamina, lerpSpeed * DELTA);

	hpBar->SetLerpValue(lerpHp);
	hpBar->SetValue(hp);
	hpBar->Update();

	staminaBar->SetLerpValue(lerpStamina);
	staminaBar->SetValue(stamina);
	staminaBar->Update();

	icon->UpdateWorld();

	if(MOUSE_CLICK(0))
	{
		ParticleManager::Get()->Play("HorizonStar", tempPos, 0);
		tempPos.x += 10;
		tempPos.z += 10;
	}

	ParticleManager::Get()->Update();

	particleSystem->Update();
	particle->Update();
}

void TestScene::PreRender()
{
}

void TestScene::Render()
{
	particleSystem->Render();
	particle->Render();
	ParticleManager::Get()->Render();
}

void TestScene::PostRender()
{
	ui->Render();
	button->Render();
	hpBar->Render();
	staminaBar->Render();
	icon->Render();
}

void TestScene::GUIRender()
{
	ui->GUIRender();
	button->GUIRender();
	ImGui::DragFloat3("mousePos", (float*)&mousePos, 1408.f);
	hpBar->GUIRender();
	staminaBar->GUIRender();
	icon->GUIRender();

	particleSystem->GUIRender();
	particle->GUIRender();
}
