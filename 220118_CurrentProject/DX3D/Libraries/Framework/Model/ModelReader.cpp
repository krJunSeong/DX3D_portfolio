#include "Framework.h"

ModelReader::ModelReader(string name)
    : name(name)
{
    ReadMaterial();
    ReadMesh();
}

ModelReader::~ModelReader()
{
    for (Material* material : materials)
        delete material;

    for (ModelMesh* mesh : meshes)
        delete mesh;
}

void ModelReader::Render()
{
    for (ModelMesh* mesh : meshes)
        mesh->Render();
}

void ModelReader::Render(UINT instanceCount)
{
    for (ModelMesh* mesh : meshes)
        mesh->Render(instanceCount);
}

void ModelReader::GUIRender()
{
    for (Material* material : materials)
        material->GUIRender();
}

void ModelReader::SetShader(wstring file)
{
    for (Material* material : materials)
    {
        material->SetShader(file);
    }
}

int ModelReader::GetBone(string name)
{
    if (boneMap.count(name) == 0) return -1;

    return boneMap[name];
}

int ModelReader::GetNode(string name)
{
    for (NodeData node : nodes)
    {
        if (node.name == name)
            return node.index;
    }

    return 0;
}

Vector3 ModelReader::GetSize()
{
    Vector3 minBox, maxBox;
    meshes[0]->SetBox(&minBox, &maxBox);

    for (UINT i = 1; i < meshes.size(); i++)
    {
        Vector3 minPos, maxPos;

        meshes[i]->SetBox(&minPos, &maxPos);

        minBox.x = min(minBox.x, minPos.x);
        minBox.y = min(minBox.y, minPos.y);
        minBox.z = min(minBox.z, minPos.z);

        maxBox.x = max(maxBox.x, maxPos.x);
        maxBox.y = max(maxBox.y, maxPos.y);
        maxBox.z = max(maxBox.z, maxPos.z);
    }

    return maxBox - minBox;
}

void ModelReader::ReadMaterial()
{
    string path = "ModelData/Materials/" + name + ".mats";

    BinaryReader r(path);

    UINT size = r.UInt();

    materials.resize(size);

    for (Material*& material : materials)
    {
        material = new Material();

        string temp = r.String();

        string path = "TextData/Materials/" + name + "/" + temp + ".mat";

        material->Load(path);
    }
}

void ModelReader::ReadMesh()
{
    string path = "ModelData/Meshes/" + name + ".mesh";

    BinaryReader r(path);

    UINT size = r.UInt();

    meshes.resize(size);
    for (ModelMesh*& mesh : meshes)
    {
        mesh = new ModelMesh(r.String());
        UINT materialIndex = r.UInt();
        mesh->SetMaterial(materials[materialIndex]);

        //vertices
        UINT vertexCount = r.UInt();

        ModelVertex* vertices = new ModelVertex[vertexCount];
        r.Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

        //indices
        UINT indexCount = r.UInt();

        UINT* indices = new UINT[indexCount];
        r.Byte((void**)&indices, sizeof(UINT) * indexCount);

        mesh->CreateMesh(vertices, vertexCount, indices, indexCount);
    }

    size = r.UInt();
    nodes.resize(size);
    for (NodeData& node : nodes)
    {
        node.index = r.Int();
        node.name = r.String();
        node.parent = r.Int();
        node.transform = r.Matrix();
    }

    size = r.UInt();
    bones.resize(size);
    for (BoneData& bone : bones)
    {
        bone.name = r.String();
        bone.index = r.Int();
        bone.offset = r.Matrix();

        boneMap[bone.name] = bone.index;
    }
}
