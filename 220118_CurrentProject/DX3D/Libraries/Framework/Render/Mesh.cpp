#include "Framework.h"

Mesh::Mesh(void* vertexData, UINT vertexStride, UINT vertexCount, void* indexData, UINT indexCount)
{
    vertexBuffer = new VertexBuffer(vertexData, vertexStride, vertexCount);
    indexBuffer = new IndexBuffer(indexData, indexCount);
}

Mesh::~Mesh()
{
    delete vertexBuffer;
    delete indexBuffer;
}

void Mesh::IASet(D3D11_PRIMITIVE_TOPOLOGY type)
{
    vertexBuffer->IASet(type);
    indexBuffer->IASet();
}

void Mesh::UpdateVertex(void* data, UINT count)
{
    vertexBuffer->Update(data, count);
}
