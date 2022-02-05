#include "framework.h"

ReportGrid::ReportGrid()
	:width(30), height(30)
{
	// SamplerState는 Environment에서 설정함.
	material = new Material(L"Tutorial.hlsl");

	//material->SetDiffuseMap(L"Textures/Landscape/Tree.png");

	CreateMesh();

	worldBuffer = new MatrixBuffer();
	colorBuffer = new ColorBuffer();
}

ReportGrid::~ReportGrid()
{
	delete material;
	delete mesh;

	delete worldBuffer;
	delete colorBuffer;
}

void ReportGrid::Update()
{
}

void ReportGrid::Render()
{
	worldBuffer->SetVSBuffer(0);
	colorBuffer->SetPSBuffer(10);

	mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);				// D3D11_PRIMITIVE_TOPOLOGY, indexBuffer 
	material->Set();			// Shader

	DC->DrawIndexed(indices.size(), 0, 0);
}

void ReportGrid::CreateMesh()
{
	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{	
			//VertexType vertex;
			//vertex.position = {(float)x - 10, -2, (float)z - 10};		// 카메라에 안 보여서 -10 위치조정
			
			Float3 color;
			if(width * 0.5  - 10 == 0) color = {0,1,1};
			else if(height * 0.5 - 10 == 0) color = {1,0,1};
			else color = {1,1,1};

			vertices.emplace_back((float)x - 10, -2, (float)z - 10, color.x, color.y, color.z);
		}
	}
	
	for(UINT z = 0; z < height; z++)
	{
		indices.push_back(width * z);
		indices.push_back((height - 1) + z * height);

		indices.push_back(z);
		indices.push_back((height - 1) * height + z);
	}

	// 가운데 중앙 세로선 예외처리
	//					                X           Y             Z                Color
	vertices.emplace_back((float)width * 0.5 - 10, -10, (float)height * 0.5 - 10, 0, 0, 1);
	vertices.emplace_back((float)width * 0.5 - 10, +10, (float)height * 0.5 - 10, 0, 0, 1);

	indices.push_back(vertices.size() -2);
	indices.push_back(vertices.size() -1);


	
	
	//Indices
	//for (UINT z = 0; z < height - 1; z++)
	//{
	//	for (UINT x = 0; x < width - 1; x++)
	//	{
	//		indices.push_back(width * z + x);			// 0 (0)
	//		indices.push_back(width * (z + 1) + x);		// 1 (10)
	//		indices.push_back(width * (z + 1) + x + 1); // 2 (11)

	//		indices.push_back(width * z + x);			// 0 (0)
	//		indices.push_back(width * (z + 1) + x + 1); // 2 (11)
	//		indices.push_back(width * z + x + 1);		// 3 (1)
	//	}
	//}

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), indices.size()); //vertex, index
}

// 색 바꾸는 방법
//		colorBuffer->Set(0,0,0);