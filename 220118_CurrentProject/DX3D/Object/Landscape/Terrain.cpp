#include "framework.h"

Terrain::Terrain(wstring alphaMapName, wstring heightMapName) 
	: secondMap(nullptr), thirdMap(nullptr)
{
	wstring alphaMapPath =  L"Textures/AlphaMaps/"  + alphaMapName;
	wstring heightMapPath = L"Textures/HeightMaps/" + heightMapName;

    material = new Material(L"TerrainSplatting.hlsl");
    material->SetDiffuseMap(L"Textures/Landscape/Grace.png");

    worldBuffer = new WorldBuffer();

    heightMap = Texture::Add(heightMapPath);
    alphaMap = Texture::Add(alphaMapPath);

    secondMap = Texture::Add(L"Textures/Landscape/Dirt.png");
    //thirdMap = Texture::Add(L"Textures/Landscape/Stones.png");

    CreateMesh();
    CreateCompute();

    rasterizerState = new RasterizerState();
    rasterizerState->FillMode(D3D11_FILL_WIREFRAME);
}

Terrain::~Terrain()
{
    delete material;
    delete mesh;
    delete worldBuffer;
    delete rasterizerState;

    delete structuredBuffer;
    delete rayBuffer;
    delete[] input;
    delete[] output;
}

void Terrain::Render()
{
    worldBuffer->Set(world);
    worldBuffer->SetVSBuffer(0);

    alphaMap->Set(10);
    if (secondMap)
        secondMap->Set(11);
    if (thirdMap)
        thirdMap->Set(12);

    mesh->IASet();
    material->Set();

    //rasterizerState->SetState();

    DC->DrawIndexed(indices.size(), 0, 0);
}

float Terrain::GetHeight(Vector3 position)
{
    int x = (int)position.x;
    int z = (int)(height - position.z - 1);

    if (x < 0 || x >= width - 1) return 0.0f;
    if (z < 0 || z >= height - 1) return 0.0f;

    UINT index[4];
    index[0] = width * (z + 1) + x;
    index[1] = width * z + x;
    index[2] = width * (z + 1) + x + 1;
    index[3] = width * z + x + 1;

    Vector3 p[4];
    for (UINT i = 0; i < 4; i++)
        p[i] = vertices[index[i]].position;

    float u = position.x - p[0].x;
    float v = position.z - p[0].z;

    Vector3 result;

    if (u + v <= 1.0f)
    {
        result = ((p[2] - p[0]) * u + (p[1] - p[0]) * v) + p[0];
    }
    else
    {
        u = 1.0f - u;
        v = 1.0f - v;

        result = ((p[1] - p[3]) * u + (p[2] - p[3]) * v) + p[3];
    }

    return result.y;
}

Vector3 Terrain::Picking()
{
    Ray ray = CAM->ScreenPointToRay(mousePos);

    rayBuffer->data.position = ray.position;
    rayBuffer->data.direction = ray.direction;
    rayBuffer->data.size = size;

    rayBuffer->SetCSBuffer(0);

    DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
    DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

    computeShader->Set();

    UINT x = ceil((float)size / 64.0f);

    DC->Dispatch(x, 1, 1);

    structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

    float minDistance = FLT_MAX;
    int minIndex = -1;

    for (UINT i = 0; i < size; i++)
    {
        OutputDesc temp = output[i];
        if (temp.picked)
        {
            if (minDistance > temp.distance)
            {
                minDistance = temp.distance;
                minIndex = i;
            }
        }
    }

    if (minIndex >= 0)
    {
        return ray.position + ray.direction * minDistance;
    }

    return Vector3();
}

void Terrain::CreateMesh()
{
    width = heightMap->Width();
    height = heightMap->Height();

    vector<Float4> pixels = heightMap->ReadPixels();

    //Vertices
    for (UINT z = 0; z < height; z++)
    {
        for (UINT x = 0; x < width; x++)
        {
            VertexType vertex;
            vertex.position = { (float)x, 0.0f, width - (float)z - 1.0f };
            vertex.uv.x = x / (float)(width - 1);
            vertex.uv.y = z / (float)(height - 1);

            //Float4 color = pixels[vertices.size()];
            //vertex.position.y = color.x * MAX_HEIGHT;
            UINT index = width * z + x;
            vertex.position.y = pixels[index].x * MAX_HEIGHT;

            vertices.emplace_back(vertex);
        }
    }

    //Indices
    for (UINT z = 0; z < height - 1; z++)
    {
        for (UINT x = 0; x < width - 1; x++)
        {
            indices.push_back(width * z + x);//0            
            indices.push_back(width * (z + 1) + x + 1);//2
            indices.push_back(width * (z + 1) + x);//1

            indices.push_back(width * z + x);//0            
            indices.push_back(width * z + x + 1);//3
            indices.push_back(width * (z + 1) + x + 1);//2
        }
    }

    CreateNormal();

    mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
        indices.data(), indices.size());
}

void Terrain::CreateNormal()
{
    for (UINT i = 0; i < indices.size() / 3; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].position;
        Vector3 v1 = vertices[index1].position;
        Vector3 v2 = vertices[index2].position;

        Vector3 A = v1 - v0;
        Vector3 B = v2 - v0;

        Vector3 normal = Vector3::Cross(A, B).GetNormalized();

        vertices[index0].normal = normal + vertices[index0].normal;
        vertices[index1].normal = normal + vertices[index1].normal;
        vertices[index2].normal = normal + vertices[index2].normal;
    }
}

void Terrain::CreateCompute()
{
    computeShader = Shader::AddCS(L"ComputePicking.hlsl");

    size = indices.size() / 3;

    input = new InputDesc[size];
    output = new OutputDesc[size];

    for (UINT i = 0; i < size; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        input[i].v0 = vertices[index0].position;
        input[i].v1 = vertices[index1].position;
        input[i].v2 = vertices[index2].position;

        input[i].index = i;
    }

    structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size,
        sizeof(OutputDesc), size);

    rayBuffer = new RayBuffer();
}
