#include "framework.h"
#include "BillboardScene.h"

BillboardScene::BillboardScene()
{
	terrain = new Terrain();
	/*
	for(UINT i = 0; i < COUNT; i++)
	{
		Quad* tree = new Quad();
		tree->GetMaterial()->SetDiffuseMap(L"Textures/Landscape/Tree.png");
		tree->GetMaterial()->SetShader(L"Texture.hlsl"); // 알파값 쉐이더에서 맥이기 위한 조치

		Vector3 pos;
		pos.x = Random(0.0f, terrain->GetSize().x);
		pos.z = Random(0.0f, terrain->GetSize().y);

		Vector3 scale;
		scale.x = Random(3.0f, 10.0f);
		scale.y = Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = terrain->GetHeight(pos) + scale.y * 0.5f;
		
		tree->position = pos;
		tree->scale = scale;
		
		trees.push_back(tree);
	}

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	//blendState[1]->AlphaToCoverage(true); //AlphaToCoverage: 알파값이 0.5 이하면 무조건 투명
	blendState[1]->Alpha(true); //AlphaToCoverage: 알파값이 0.5 이하면 무조건 투명

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
	*/

	// ------------------- Teacher --------------------
	material = new Material(L"Geometry/Billboard.hlsl");
	material->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	geometryShader = Shader::AddGS(L"Geometry/Billboard.hlsl");

	VertexSize* vertices = new VertexSize[COUNT];

	for (UINT i = 0; i < COUNT; i++)
	{
		Vector3 pos;
		pos.x = Random(0.0f, terrain->GetSize().x);
		pos.z = Random(0.0f, terrain->GetSize().y);

		Vector3 scale;
		scale.x = Random(3.0f, 10.0f);
		scale.y = Random(3.0f, 10.0f);
		scale.z = 1.0f;

		pos.y = terrain->GetHeight(pos) + scale.y * 0.5f;

		vertices[i].position = pos;
		vertices[i].size = { scale.x, scale.y };
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), COUNT);
	delete[] vertices;

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	//blendState[1]->AlphaToCoverage(true);
	blendState[1]->Alpha(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

BillboardScene::~BillboardScene()
{
	/*
	delete terrain;

	for(Quad* tree : trees)
		delete tree;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
	*/
	delete terrain;

	delete material;
	delete vertexBuffer;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void BillboardScene::Update()
{
	/*
	for (Quad* tree : trees)
	{
		// 카메라 바라보게 하는 방법1
		//tree->rotation.y = CAM->rotation.y; // 카메라만 바라보게 하는 코드
		//tree->rotation.x = CAM->rotation.x;  // 카메라만 바라보게 하는 코드(x축도)

		// 카메라 바라보게 하는 방법2
		Vector3 dir = tree->position - CAM->position;
		tree->rotation.y = atan2(dir.x, dir.z);

		tree->UpdateWorld();
	}*/
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	/*terrain->Render();

	blendState[1]->SetState();
	depthState[1]->SetState();

	for (Quad* tree : trees)
		tree->Render();

	blendState[0]->SetState();
	depthState[0]->SetState();*/

	terrain->Render();

	blendState[1]->SetState();
	depthState[1]->SetState();

	//for (Quad* tree : trees)
		//tree->Render();

	vertexBuffer->IASet(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->Set();

	DC->Draw(COUNT, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void BillboardScene::PostRender()
{
}

void BillboardScene::GUIRender()
{
}
