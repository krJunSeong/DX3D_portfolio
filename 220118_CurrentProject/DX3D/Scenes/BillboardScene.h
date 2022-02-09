#pragma once

class BillboardScene : public Scene
{
private:
	const UINT COUNT = 1000;

	Terrain* terrain;

	// ---------------------
	//vector<Quad*> trees;
	
	//BlendState* blendState[2]; // 알파값 처리
	//DepthStencilState* depthState[2];

	// ------------- Teacher --------------------
	Material* material;
	VertexBuffer* vertexBuffer;

	GeometryShader* geometryShader;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];

public:
	BillboardScene();
	~BillboardScene();

private:
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};

// IA -> VS -> TS, GS -> RS -> PS -> OM
// Geomatry State를 왜쓰냐? Geometry: 기하학
// DX10 도입, 정점을 조작하는 역할

// VS: 좌표계
// GS: 정점을 늘리거나 줄일 수 있음.