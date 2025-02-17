#pragma once

class ModelExportScene : public Scene
{
private:
	ModelExporter* exporter;

public:
	ModelExportScene();
	~ModelExportScene();

private:
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
