#include "framework.h"
#include "AstarScene.h"

AstarScene::AstarScene()
{
	terrain = new Terrain();

	aStar = new AStar();
	aStar->SetNode(terrain);

	CreateBoxes();
}

AstarScene::~AstarScene()
{
	delete terrain;
	delete aStar;
	
	for(Box* box : boxes)
		delete box;
}

void AstarScene::Update()
{
	aStar->Update();

	for (Box* box : boxes)
		box->Update();
}

void AstarScene::PreRender()
{
}

void AstarScene::Render()
{
	terrain->Render();

	aStar->Render();

	for (Box* box : boxes)
		box->Render();
}

void AstarScene::PostRender()
{
}

void AstarScene::GUIRender()
{
	terrain->GUIRender();

	for (Box* box : boxes)
		box->GUIRender();
}

void AstarScene::CreateBoxes()
{
	for(int i =0; i< 5; i++)
	{
		string tag = "Box_" + to_string(i);
		Box* box = new Box(tag);
		box->Update();

		boxes.push_back(box);

		aStar->AddObstacle(box->GetCollider());
	}

//	aStar->
}

/*
	box->update 하는 이유
		시작하자마자 충돌처리르 하기 때문에 미리 갱신을 시켜두는 것임.


*/