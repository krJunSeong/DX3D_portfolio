#include "Framework.h"

GeometryShader::GeometryShader(wstring file)
{
    wstring path = L"Shaders/" + file;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS + D3DCOMPILE_DEBUG;

    V(D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "GS", "gs_5_0", flags, 0, &blob, nullptr));

    DEVICE->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(),
        nullptr, &shader);

    blob->Release();
}

GeometryShader::~GeometryShader()
{
    shader->Release();
}

void GeometryShader::Set()
{
    DC->GSSetShader(shader, nullptr, 0);
}
