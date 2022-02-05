#pragma once

class ModelExportScene : public Scene
{
private:
	ModelExporter* exporter;

public:
	ModelExportScene();
	~ModelExportScene();

private:
	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
};
