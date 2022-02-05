#include "framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    string name = "RockShield"; // 이름 바꿀 것
    string clipName;
    string file;

    //Model 추출
        file = "ModelData/FBX/" + name + ".fbx";
        ModelExporter* exporter = new ModelExporter(name, file);
        exporter->ExportMaterial();
        exporter->ExportMesh();
        delete exporter;

    // 애니메이션 추출
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
