#pragma once

class ModelReader
{
private:
    string name;

    vector<Material*> materials;
    vector<ModelMesh*> meshes;
    vector<NodeData> nodes;
    vector<BoneData> bones;

    map<string, UINT> boneMap;
public:
    ModelReader(string name);
    ~ModelReader();

    void Render();
    void Render(UINT instanceCount);

    void GUIRender();

    void SetShader(wstring file);

    vector<NodeData>& GetNodes() { return nodes; }
    vector<BoneData>& GetBones() { return bones; }

    int GetBone(string name);
    int GetNode(string name);

    Vector3 GetSize();

    string GetName() { return name; }
private:
    void ReadMaterial();
    void ReadMesh();
};