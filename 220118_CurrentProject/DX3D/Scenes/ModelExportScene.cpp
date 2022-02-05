#include "framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    string name = "RockShield"; // �̸� �ٲ� ��
    string clipName;
    string file;

    //Model ����
        file = "ModelData/FBX/" + name + ".fbx";
        ModelExporter* exporter = new ModelExporter(name, file);
        exporter->ExportMaterial();
        exporter->ExportMesh();
        delete exporter;

    // �ִϸ��̼� ����
        //clipName = "Idle";
        //file = "ModelData/Animations/" + name + "/" + clipName + ".fbx";
        //exporter = new ModelExporter(name, file);
        //exporter->ExportClip(clipName);
        //delete exporter;
        //
        //clipName = "Run";
        //file = "ModelData/Animations/" + name + "/" + clipName + ".fbx";
        //exporter = new ModelExporter(name, file);
        //exporter->ExportClip(clipName);
        //delete exporter;
        //
        //clipName = "Death";
        //file = "ModelData/Animations/" + name + "/" + clipName + ".fbx";
        //exporter = new ModelExporter(name, file);
        //exporter->ExportClip(clipName);
        //delete exporter;
        //
        //clipName = "Hit";
        //file = "ModelData/Animations/" + name + "/" + clipName + ".fbx";
        //exporter = new ModelExporter(name, file);
        //exporter->ExportClip(clipName);
        //delete exporter;
        //
        //clipName = "Attack";
        //file = "ModelData/Animations/" + name + "/" + clipName + ".fbx";
        //exporter = new ModelExporter(name, file);
        //exporter->ExportClip(clipName);
        //delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
