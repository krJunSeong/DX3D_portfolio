#pragma once

class Terrain : public Transform
{
private:
	// r값이 1인 것을 50으로 잡음.
	const float MAX_HEIGHT = 10.0f;

	typedef VertexUVNormal VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height; // 정점갯수

	WorldBuffer* worldBuffer;

	Texture* heightMap; // 높이맵 로드
	Texture* alphaMap;

	Texture* secondMap;
	Texture* thirdMap;

	RasterizerState* rasterizerState;
public:
	Terrain();
	~Terrain();

	void Render();


	vector<VertexType> GetVertices(){return vertices;}

	float GetHeight(Vector3 position);

	UINT GetWidth() {return width;}
	UINT GetHeight() { return height; }

	Float2 GetSize() {return Float2(width, height);}
private:
	void CreateMesh();
	void CreateNormal();
};