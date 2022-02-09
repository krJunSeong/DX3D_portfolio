#include "Framework.h"

ModelMesh::ModelMesh(string name)
    : name(name), vertexCount(0), indexCount(0)
{
}

ModelMesh::~ModelMesh()
{
    delete mesh;

    delete[] vertices;
    delete[] indices;
}

void ModelMesh::Render()
{
    mesh->IASet();
    material->Set();

    DC->DrawIndexed(indexCount, 0, 0);
}

void ModelMesh::Render(UINT instanceCount)
{
    mesh->IASet();
    material->Set();

    DC->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void ModelMesh::CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount)
{
    mesh = new Mesh(vertexData, sizeof(ModelVertex), vertexCount,
        indexData, indexCount);

    this->vertexCount = vertexCount;
    this->indexCount = indexCount;
    vertices = (ModelVertex*)vertexData;
    indices = (UINT*)indices;
}

void ModelMesh::SetBox(Vector3* minBox, Vector3* maxBox)
{
    if (vertexCount == 0) return;

    Vector3 minPos = vertices[0].position;
    Vector3 maxPos = vertices[0].position;

    for (UINT i = 1; i < vertexCount; i++)
    {
        minPos.x = min(minPos.x, vertices[i].position.x);
        minPos.y = min(minPos.y, vertices[i].position.y);
        minPos.z = min(minPos.z, vertices[i].position.z);

        maxPos.x = max(maxPos.x, vertices[i].position.x);
        maxPos.y = max(maxPos.y, vertices[i].position.y);
        maxPos.z = max(maxPos.z, vertices[i].position.z);
    }

    *minBox = minPos;
    *maxBox = maxPos;
}

