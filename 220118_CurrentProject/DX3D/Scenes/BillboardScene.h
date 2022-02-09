#pragma once

class BillboardScene : public Scene
{
private:
	const UINT COUNT = 1000;

	Terrain* terrain;

	// ---------------------
	//vector<Quad*> trees;
	
	//BlendState* blendState[2]; // ���İ� ó��
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
	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};

// IA -> VS -> TS, GS -> RS -> PS -> OM
// Geomatry State�� �־���? Geometry: ������
// DX10 ����, ������ �����ϴ� ����

// VS: ��ǥ��
// GS: ������ �ø��ų� ���� �� ����.