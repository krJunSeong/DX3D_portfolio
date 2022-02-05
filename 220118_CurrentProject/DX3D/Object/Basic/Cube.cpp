#include "framework.h"

Cube::Cube()
{
	material = new Material(L"Lighting.hlsl");		// VertexShader, PixelShader, Texture*

	//CreateTextureMesh();
	CreateSmoothMesh();

	worldBuffer = new WorldBuffer();	// VertexBuffer, IndexBuffer*
}

Cube::~Cube()
{
	delete material;
	delete mesh;
	delete worldBuffer;
}

void Cube::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVSBuffer(0);

	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Cube::CreateTextureMesh()
{
	//front
	vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0, 1);//0
	vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0, 0);//1
	vertices.emplace_back(+0.5f, -0.5f, -0.5f, 1, 1);//2
	vertices.emplace_back(+0.5f, +0.5f, -0.5f, 1, 0);//3
	
	//back
	vertices.emplace_back(-0.5f, -0.5f, +0.5f, 0, 1);//4
	vertices.emplace_back(-0.5f, +0.5f, +0.5f, 0, 0);//5
	vertices.emplace_back(+0.5f, -0.5f, +0.5f, 1, 1);//6
	vertices.emplace_back(+0.5f, +0.5f, +0.5f, 1, 0);//7

	//UP
	vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0, 1);//8
	vertices.emplace_back(+0.5f, +0.5f, -0.5f, 0, 0);//9
	vertices.emplace_back(-0.5f, +0.5f, +0.5f, 1, 1);//10
	vertices.emplace_back(+0.5f, +0.5f, +0.5f, 1, 0);//11

	//DOWN
	vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0, 1);//12
	vertices.emplace_back(+0.5f, -0.5f, -0.5f, 0, 0);//13
	vertices.emplace_back(-0.5f, -0.5f, +0.5f, 1, 1);//14
	vertices.emplace_back(+0.5f, -0.5f, +0.5f, 1, 0);//15

	//Right
	vertices.emplace_back(+0.5f, -0.5f, -0.5f, 0, 1);//16
	vertices.emplace_back(+0.5f, +0.5f, -0.5f, 0, 0);//17
	vertices.emplace_back(+0.5f, -0.5f, +0.5f, 1, 1);//18
	vertices.emplace_back(+0.5f, +0.5f, +0.5f, 1, 0);//19

	//LEFT
	vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0, 1);//20
	vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0, 0);//21
	vertices.emplace_back(-0.5f, -0.5f, +0.5f, 1, 1);//22
	vertices.emplace_back(-0.5f, +0.5f, +0.5f, 1, 0);//23

	//Right
	indices = {
		// Front
		0, 1, 2, 2, 1, 3,
		// back
		4, 6, 5, 6, 7, 5,
		// UP
		8, 10, 9, 9, 10, 11,
		// bottom
		12, 13, 14, 14, 13, 15,
		// right
		16, 17, 18, 18, 17, 19,
		//Left
		20, 22, 21, 21, 22, 23
	};

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

// --------------- Report ----------------------
Cube::Cube(Float3 size)
{
	material = new Material();			// VertexShader, PixelShader, Texture*

	CreateMesh(size);

	worldBuffer = new WorldBuffer();	// VertexBuffer, IndexBuffer*
}

void Cube::CreateMesh(Float3 size)
{
	Cube::size = size;
	float halfX = size.x * 0.5;
	float halfY = size.y * 0.5;
	float halfZ = size.z * 0.5;

	//front
	vertices.emplace_back(-halfX, -halfY, -halfZ, 0, 1);//0
	vertices.emplace_back(-halfX, +halfY, -halfZ, 0, 0);//1
	vertices.emplace_back(+halfX, -halfY, -halfZ, 1, 1);//2
	vertices.emplace_back(+halfX, +halfY, -halfZ, 1, 0);//3
										   
	//back								   
	vertices.emplace_back(-halfX, -halfY, +halfZ, 0, 1);//4
	vertices.emplace_back(-halfX, +halfY, +halfZ, 0, 0);//5
	vertices.emplace_back(+halfX, -halfY, +halfZ, 1, 1);//6
	vertices.emplace_back(+halfX, +halfY, +halfZ, 1, 0);//7
										   
	//UP								   
	vertices.emplace_back(-halfX, +halfY, -halfZ, 0, 1);//8
	vertices.emplace_back(+halfX, +halfY, -halfZ, 0, 0);//9
	vertices.emplace_back(-halfX, +halfY, +halfZ, 1, 1);//10
	vertices.emplace_back(+halfX, +halfY, +halfZ, 1, 0);//11
										   
	//DOWN								   
	vertices.emplace_back(-halfX, -halfY, -halfZ, 0, 1);//12
	vertices.emplace_back(+halfX, -halfY, -halfZ, 0, 0);//13
	vertices.emplace_back(-halfX, -halfY, +halfZ, 1, 1);//14
	vertices.emplace_back(+halfX, -halfY, +halfZ, 1, 0);//15
								   		   
	//Right						   		   
	vertices.emplace_back(+halfX, -halfY, -halfZ, 0, 1);//16
	vertices.emplace_back(+halfX, +halfY, -halfZ, 0, 0);//17
	vertices.emplace_back(+halfX, -halfY, +halfZ, 1, 1);//18
	vertices.emplace_back(+halfX, +halfY, +halfZ, 1, 0);//19
								   		   
	//LEFT						   		   
	vertices.emplace_back(-halfX, -halfY, -halfZ, 0, 1);//20
	vertices.emplace_back(-halfX, +halfY, -halfZ, 0, 0);//21
	vertices.emplace_back(-halfX, -halfY, +halfZ, 1, 1);//22
	vertices.emplace_back(-halfX, +halfY, +halfZ, 1, 0);//23

	//Right
	indices = {
		// Front
		0, 1, 2, 2, 1, 3,
		// back
		4, 6, 5, 6, 7, 5,
		// UP
		8, 10, 9, 9, 10, 11,
		// bottom
		12, 13, 14, 14, 13, 15,
		// right
		16, 17, 18, 18, 17, 19,
		//Left
		20, 22, 21, 21, 22, 23
	};

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

// ----------------------------------------------------


void Cube::CreateSmoothMesh()
{
	//front
	vertices.emplace_back(-0.5f, -0.5f, -0.5f, 0, 1);//0
	vertices.emplace_back(-0.5f, +0.5f, -0.5f, 0, 0);//1
	vertices.emplace_back(+0.5f, -0.5f, -0.5f, 1, 1);//2
	vertices.emplace_back(+0.5f, +0.5f, -0.5f, 1, 0);//3

	//back
	vertices.emplace_back(-0.5f, -0.5f, +0.5f, 0, 1);//4
	vertices.emplace_back(-0.5f, +0.5f, +0.5f, 0, 0);//5
	vertices.emplace_back(+0.5f, -0.5f, +0.5f, 1, 1);//6
	vertices.emplace_back(+0.5f, +0.5f, +0.5f, 1, 0);//7

	indices = {
		
		0, 1, 2, 2, 1, 3,	// Front
		1, 5, 3, 3, 5, 7,	// up
		2, 3, 7, 2, 7, 6,	// right
		0, 4, 1, 1, 4, 5,	// left
		4, 6, 5, 5, 6, 7,	// back
		0, 2, 4, 4, 2, 6	// down
	};

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), vertices.size(),
		indices.data(), indices.size());
}

void Cube::CreateNormal()
{
	// 폴리곤 수
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		/*
			폴리곤별 법선을 구하자!
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
