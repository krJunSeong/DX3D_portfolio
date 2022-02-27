#include "framework.h"
#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
	CreateUI();
}

PlayerUI::~PlayerUI()
{
	delete ui;
	delete button;
	delete hpBar;
	delete icon;
	delete staminaBar;
}

void PlayerUI::Update()
{
	ui->Update();
	button->Update();

	if (KEY_DOWN(VK_SPACE))
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
}

void PlayerUI::Render()
{
	ui->Render();
	button->Render();
	hpBar->Render();
	staminaBar->Render();
	icon->Render();
}

void PlayerUI::GUIRender()
{
	ui->GUIRender();
	button->GUIRender();
	ImGui::DragFloat3("mousePos", (float*)&mousePos, 1408.f);
	hpBar->GUIRender();
	staminaBar->GUIRender();
	icon->GUIRender();
}

void PlayerUI::CreateUI()
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
}
