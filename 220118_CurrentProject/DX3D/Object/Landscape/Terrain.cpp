#include "framework.h"

Terrain::Terrain() : secondMap(nullptr), thirdMap(nullptr)
{
	material = new Material(L"TerrainSplatting.hlsl");
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");

	worldBuffer = new WorldBuffer();

	// 높이맵 로드
	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");
	alphaMap = Texture::Add(L"Textures/AlphaMaps/TestAlpha.png");

	secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
	thirdMap = Texture::Add(L"Textures/Landscape/Stones.png");

	CreateMesh();

	rasterizerState = new RasterizerState();
	rasterizerState->FillMode(D3D11_FILL_WIREFRAME);
}

Terrain::~Terrain()
{
	delete material;
	delete mesh;
	delete worldBuffer;
	delete rasterizerState;
}

void Terrain::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	alphaMap->Set(10);
	if(secondMap)
		secondMap->Set(11);
	if(thirdMap)
		thirdMap->Set(12);

	// Texture2D register Set
	alphaMap->Set(10); 
	secondMap->Set(11);

	mesh->IASet();
	material->Set();

	//rasterizerState->SetState();

	DC->DrawIndexed(indices.size(), 0, 0);
}

float Terrain::GetHeight(Vector3 position)
{
	// position: 로봇의 위치
	// 그게 어디 인덱스에 해당하는지 확인
	// 정수형으로 바꾸면 소수점 잘리면서 왼쪽아래로 잡힘.
	int x = (int)position.x;
	int z = (int)height - position.z - 1;

	// 큐브가 맵을 벗어날 수도 있으니 예외처리
	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= height - 1) return 0.0f;

	// 플레이어 위치의 인덱스
	UINT index[4];
	index[0] = width * (z + 1) + x;		    // 좌하 
	index[1] = width * z + x;				// 좌상
	index[2] = width * (z + 1) + x + 1;		// 우상
	index[3] = width * z + x + 1;			// 우하

	// 인덱스에 대한 포지션값
	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
	{
		p[i] = vertices[index[i]].position;
	}

	// 비율 0 ~ 1 사이의 소수점값 구하기.
	float u = position.x - p[0].x;	// 0.7: 0 ~ 1사이값
	float v = position.z - p[0].z;	// 

	Vector3 result;

	if (u + v <= 1.0f)
	{
		result = (p[2] - p[0]) * u + (p[1] - p[0]) * v + p[0];
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		// p[3] 기준점
		//			가로					세로				+ 기준점
		result = (p[1] - p[3]) * u + (p[2] - p[3]) * v + p[3];
	}

	return result.y; // 높이값 반환
}

void Terrain::CreateMesh()
{
	// 높이: 높이맵 텍스처에 있는 픽셀수만큼 할 거임
	width = heightMap->Width();
	height = heightMap->Height();

	vector<Float4> pixels = heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			// 좌상단을 0,0 으로 맞춰주는 게 더 편하다.
			// 1- 하는 것도 맞춘 거긴한데 이럼 다음에 할 때도 계속 해줘야 하니까.
			vertex.position = { (float)x, 0, width - (float)z - 1};
			vertex.uv = {(float)x / (width - 1), (float)z / (height - 1) };

			// 방법 1
			//Float4 color = pixels[vertices.size()];		// R값 뽑는다.			
			//vertex.position.y = color.x  * MAX_HEIGHT;  // color는 0 ~ 1의 값이다. 그래서 maxhieght로 크기를 보정해준다.

			// 방법 2
			UINT index = width * z + x;
			vertex.position.y = pixels[index].x * MAX_HEIGHT;

			vertices.emplace_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices.push_back(width * z + x);			// 0 (0)
			indices.push_back(width * (z + 1) + x + 1); // 2 (11)
			indices.push_back(width * (z + 1) + x);		// 1 (10)

			indices.push_back(width * z + x);			// 0 (0)
			indices.push_back(width * z + x + 1);		// 3 (1)
			indices.push_back(width * (z + 1) + x + 1); // 2 (11)
		}
	}
	// 인덱스 만들고 나서 호출해줘야 함

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), indices.size());
}

void Terrain::CreateNormal()
{
	// 폴리곤 수
	for(UINT i = 0; i < indices.size() / 3; i++)
	{
		/*
			폴리곤 별 법선을 구하자!
		*/
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0; // 0 -> 1
		Vector3 B = v2 - v0; // 0 -> 2

		Vector3 normal = Vector3::Cross(A, B).GetNormalized();

		// 아직 정규화 안 된 상태. Shader에서 해줄 거임.
		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}
