#include "framework.h"
#include "TestScene.h"

TestScene::TestScene()
{
	ui = new UIPanel();
	ui->tag = "panel";
}

TestScene::~TestScene()
{
	delete ui;
}

void TestScene::Update()
{
	ui->Update();
}

void TestScene::PreRender()
{
}

void TestScene::Render()
{
}

void TestScene::PostRender()
{
	ui->Render();
}

void TestScene::GUIRender()
{
	ui->GUIRender();
	ImGui::DragFloat3("mousePos", (float*)&mousePos, 1408.f);
}
