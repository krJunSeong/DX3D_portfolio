#pragma once

class GridScene : public Scene
{
private:
	//typedef VertexUV VertexType;

	//Material* material;
	//Mesh* mesh;

	//vector<VertexType> vertices;
	//vector<UINT> indices;

	//UINT width, height; // 정점갯수

	//MatrixBuffer* worldBuffer;
	//ColorBuffer* colorBuffer;

	//RasterizerState* rasterizerState;

	Grid* grid;
public:
	GridScene();
	~GridScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateMesh();

};