#include "framework.h"
#include "GridScene.h"

GridScene::GridScene()
	//:width(10), height(10)
{
	//// SamplerState는 Environment에서 설정함.
	//material = new Material(L"Texture.hlsl");
	//material->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	//CreateMesh();

	//worldBuffer= new MatrixBuffer();
	//colorBuffer = new ColorBuffer();

	//rasterizerState = new RasterizerState();
	//rasterizerState->FillMode(D3D11_FILL_WIREFRAME);

	grid = new Grid();
}

GridScene::~GridScene()
{
	/*delete material;
	delete mesh;

	delete worldBuffer;
	delete colorBuffer;

	delete rasterizerState;*/

	delete grid;
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	//worldBuffer->SetVSBuffer(0);
	//colorBuffer->SetPSBuffer(10);

	//mesh->IASet();		// D3D11_PRIMITIVE_TOPOLOGY, indexBuffer
	//material->Set();	// Shader

	//rasterizerState->SetState();

	//DC->DrawIndexed(indices.size(), 0, 0);

	grid->Render();
}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
	grid->SetSize();
}

void GridScene::CreateMesh()
{
	////Vertices
	//for(UINT z = 0; z < height; z++)
	//{
	//	for(UINT x = 0; x < width; x++)
	//	{
	//		VertexType vertex;
	//		vertex.position = {(float)x, 0, (float)z};
	//		vertex.uv = {1 - (float)x / (width -1), 1 - (float)z / (height - 1)};

	//		vertices.emplace_back(vertex);
	//	}
	//}

	////Indices
	//for(UINT z = 0; z < height - 1; z++)
	//{
	//	for(UINT x = 0; x  < width - 1; x++)
	//	{
	//		indices.push_back(width * z + x);			// 0 (0)
	//		indices.push_back(width * (z + 1) + x);		// 1 (10)
	//		indices.push_back(width * (z + 1) + x + 1); // 2 (11)

	//		indices.push_back(width * z + x);			// 0 (0)
	//		indices.push_back(width * (z + 1) + x + 1); // 2 (11)
	//		indices.push_back(width * z + x + 1);		// 3 (1)
	//	}
	//}

	//mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
	//	indices.data(), indices.size());
}
