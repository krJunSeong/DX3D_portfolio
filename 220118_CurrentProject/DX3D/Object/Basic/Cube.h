#pragma once

class Cube : public Transform
{
protected:
	typedef VertexUVNormal VertexType;

	Material* material; // VertexShader, PixelShader, Texture*
	Mesh* mesh;			// VertexBuffer, IndexBuffer*
	
	vector<VertexType> vertices;
	vector<UINT> indices;

	WorldBuffer* worldBuffer;

	// ------------ report ------------
	Float3 size;
public:
	Cube();
	Cube(Float3 size);

	~Cube();

	void Render();
	
	Material* GetMaterial() { return material;}
private:
	void CreateTextureMesh();
	void CreateMesh(Float3 size);
	void CreateNormal();

	void CreateSmoothMesh();
};
