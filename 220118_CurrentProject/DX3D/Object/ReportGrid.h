#pragma once

class ReportGrid
{
private:
	typedef VertexColor VertexType;

	Material* material;
	Mesh* mesh;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height; // Á¤Á¡°¹¼ö

	MatrixBuffer* worldBuffer;
	ColorBuffer* colorBuffer;
public:
	ReportGrid();
	~ReportGrid();

	void Update();
	void Render();
private:
	void CreateMesh();

};