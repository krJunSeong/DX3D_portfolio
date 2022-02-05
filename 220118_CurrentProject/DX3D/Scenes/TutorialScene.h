#pragma once

class TutorialScene : public Scene
{
private:

	//Mesh: 망, vertex, index를 합쳐서 부르는 용어
	vector<ReportCube*> cubes;
	MatrixBuffer* worldBuffer;
	MatrixBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	Texture* texture;
public:
	TutorialScene();
	~TutorialScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};