#pragma once

class TutorialScene : public Scene
{
private:

	//Mesh: ��, vertex, index�� ���ļ� �θ��� ���
	vector<ReportCube*> cubes;
	MatrixBuffer* worldBuffer;
	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	Texture* texture;
public:
	TutorialScene();
	~TutorialScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};