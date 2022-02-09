#pragma once
//VertexLayouts

struct Vertex
{
    Float3 position;

    Vertex() : position(0, 0, 0)
    {}

    Vertex(float x, float y, float z) : position(x, y, z)
    {}
};

struct VertexColor
{
    Float3 position;
    Float4 color;

    VertexColor() : position(0, 0, 0), color(1, 1, 1, 1)
    {}

    VertexColor(float x, float y, float z, float r, float g, float b)
        : position(x, y, z), color(r, g, b, 1)
    {}
};

struct VertexUV
{
    Float3 position;
    Float2 uv;

    VertexUV() : position(0, 0, 0), uv(0, 0)
    {}

    VertexUV(float x, float y, float z, float u, float v)
        : position(x, y, z), uv(u, v)
    {}
};

struct VertexSize
{
    Float3 position;
    Float2 size;

    VertexSize() : position(0, 0, 0), size(0, 0)
    {}

    VertexSize(float x, float y, float z, float w, float h)
        : position(x, y, z), size(w, h)
    {}
};

struct VertexUVNormal
{
    Float3 position;
    Float2 uv;
    Float3 normal;

    VertexUVNormal() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0)
    {}
    VertexUVNormal(float x, float y, float z, float u, float v)
        : position(x, y, z), uv(u, v), normal(0, 0, 0)
    {}
};

struct VertexUVNormalTangent
{
    Float3 position;
    Float2 uv;
    Float3 normal;
    Float3 tangent;

    VertexUVNormalTangent() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0)
    {}
    VertexUVNormalTangent(float x, float y, float z, float u, float v)
        : position(x, y, z), uv(u, v), normal(0, 0, 0), tangent(0, 0, 0)
    {}
};

struct VertexUVNormalTangentAlpha
{
    Float3 position;
    Float2 uv;
    Float3 normal;
    Float3 tangent;
    float alpha[4];

    VertexUVNormalTangentAlpha() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0), alpha{}
    {}
};

struct VertexUVNormalTangentBlend
{
    Float3 position;
    Float2 uv;
    Float3 normal;
    Float3 tangent;
    Float4 indices;
    Float4 weights;

    VertexUVNormalTangentBlend() : position(0, 0, 0), uv(0, 0), normal(0, 0, 0),
        tangent(0, 0, 0), indices(0, 0, 0, 0), weights(0, 0, 0, 0)
    {}
};

struct InstanceData
{
    Matrix transform;
    UINT index;

    InstanceData() : index(0)
    {
        transform = XMMatrixIdentity();
    }
};