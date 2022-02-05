#include "framework.h"

Terrain::Terrain() : secondMap(nullptr), thirdMap(nullptr)
{
	material = new Material(L"TerrainSplatting.hlsl");
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");

	worldBuffer = new WorldBuffer();

	// ���̸� �ε�
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
	// position: �κ��� ��ġ
	// �װ� ��� �ε����� �ش��ϴ��� Ȯ��
	// ���������� �ٲٸ� �Ҽ��� �߸��鼭 ���ʾƷ��� ����.
	int x = (int)position.x;
	int z = (int)height - position.z - 1;

	// ť�갡 ���� ��� ���� ������ ����ó��
	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= height - 1) return 0.0f;

	// �÷��̾� ��ġ�� �ε���
	UINT index[4];
	index[0] = width * (z + 1) + x;		    // ���� 
	index[1] = width * z + x;				// �»�
	index[2] = width * (z + 1) + x + 1;		// ���
	index[3] = width * z + x + 1;			// ����

	// �ε����� ���� �����ǰ�
	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
	{
		p[i] = vertices[index[i]].position;
	}

	// ���� 0 ~ 1 ������ �Ҽ����� ���ϱ�.
	float u = position.x - p[0].x;	// 0.7: 0 ~ 1���̰�
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

		// p[3] ������
		//			����					����				+ ������
		result = (p[1] - p[3]) * u + (p[2] - p[3]) * v + p[3];
	}

	return result.y; // ���̰� ��ȯ
}

void Terrain::CreateMesh()
{
	// ����: ���̸� �ؽ�ó�� �ִ� �ȼ�����ŭ �� ����
	width = heightMap->Width();
	height = heightMap->Height();

	vector<Float4> pixels = heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			VertexType vertex;
			// �»���� 0,0 ���� �����ִ� �� �� ���ϴ�.
			// 1- �ϴ� �͵� ���� �ű��ѵ� �̷� ������ �� ���� ��� ����� �ϴϱ�.
			vertex.position = { (float)x, 0, width - (float)z - 1};
			vertex.uv = {(float)x / (width - 1), (float)z / (height - 1) };

			// ��� 1
			//Float4 color = pixels[vertices.size()];		// R�� �̴´�.			
			//vertex.position.y = color.x  * MAX_HEIGHT;  // color�� 0 ~ 1�� ���̴�. �׷��� maxhieght�� ũ�⸦ �������ش�.

			// ��� 2
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
	// �ε��� ����� ���� ȣ������� ��

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), indices.size());
}

void Terrain::CreateNormal()
{
	// ������ ��
	for(UINT i = 0; i < indices.size() / 3; i++)
	{
		/*
			������ �� ������ ������!
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

		// ���� ����ȭ �� �� ����. Shader���� ���� ����.
		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}
