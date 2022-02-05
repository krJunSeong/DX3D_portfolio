#include "Framework.h"

map<wstring, Texture*> Texture::textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
    : srv(srv), image(move(image))
{
}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if (textures.count(file) > 0)
        return textures[file];

    ScratchImage image;

    wstring extension = GetExtension(file);

    if (extension.compare(L"tga") == 0)
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    ID3D11ShaderResourceView* srv;

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    textures[file] = new Texture(srv, image);
    textures[file]->file = file;

    return textures[file];
}

Texture* Texture::Add(wstring file, wstring key)
{
    if (textures.count(key) > 0)
        return textures[key];

    ScratchImage image;

    wstring extension = GetExtension(file);

    if (extension.compare(L"tga") == 0)
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);

    ID3D11ShaderResourceView* srv;

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    textures[key] = new Texture(srv, image);
    textures[key]->file = file;

    return textures[key];
}

void Texture::Delete()
{
    for (pair<wstring, Texture*> texture : textures)
        delete texture.second;
}

void Texture::Set(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

vector<Float4> Texture::ReadPixels()
{
    uint8_t* colors = image.GetPixels();
    UINT size = image.GetPixelsSize();

    float scale = 1.0f / 255.0f;

    vector<Float4> result(size / 4);

    for (UINT i = 0; i < result.size(); i++)
    {
        result[i].x = colors[i * 4 + 0] * scale;
        result[i].y = colors[i * 4 + 1] * scale;
        result[i].z = colors[i * 4 + 2] * scale;
        result[i].w = colors[i * 4 + 3] * scale;
    }

    return result;
}



/*
#include "framework.h"

map<wstring, Texture*> Texture::textures;

Texture::Texture(ID3D11ShaderResourceView* srv, ScratchImage& image)
    : srv(srv), image(move(image))  // 이동생성자
{
}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if(textures.count(file) > 0)
        return textures[file]; // 파일 있으면 있는 거 반환

    ScratchImage image;

    wstring extension = GetExtension(file);  // 확장자를 받아올게 LoadFromWICfile은 jpg 같은 것만 읽거든

    // ------------------------ 확장자 읽는 함수 예외처리 ------------------------
    if(extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if(extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(),WIC_FLAGS_FORCE_RGB, nullptr, image);


    ID3D11ShaderResourceView* srv;

    // 이미지 경로 잘못 넣었을 때 여기서 터트린다.
    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    textures[file] = new Texture(srv, image);

    return textures[file];
}

Texture* Texture::Add(wstring file, wstring key)
{
    if (textures.count(key) > 0)
        return textures[key]; // 파일 있으면 있는 거 반환

    ScratchImage image;

    wstring extension = GetExtension(file);  // 확장자를 받아올게 LoadFromWICfile은 jpg 같은 것만 읽거든

    // ------------------------ 확장자 읽는 함수 예외처리 ------------------------
    if (extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);


    ID3D11ShaderResourceView* srv;

    // 이미지 경로 잘못 넣었을 때 여기서 터트린다.
    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    textures[key] = new Texture(srv, image);
    textures[key]->file = file;

    return textures[key];
}

void Texture::Delete()
{
    for(pair<wstring, Texture*> texture : textures)
        delete texture.second;

}

void Texture::Set(UINT slot)
{
    DC->PSSetShaderResources(slot, 1, &srv);
}

vector<Float4> Texture::ReadPixels()
{
    // uint: -128 ~ 127,  uint: 0 ~ 255
    
    uint8_t* colors = image.GetPixels(); // 배열을 받음, 하나씩만 들어감.
    UINT size = image.GetPixelsSize();  // 배열크기

    float scale = 1.0f / 255.0f; // 범위 0 ~ 255를 0 ~ 1로 바꾸는 작업.

    // colors 4개가 모여야 Float4가 됨
    // 컬러의 최종갯수는 사이즈에서 4로 나눈값이 된다.
    vector<Float4> result(size / 4);

    // Float[0...]의  rgba 값을 뽑아준다.
    for(UINT i = 0; i < result.size(); i++)
    {
        result[i].x = colors[i * 4 + 0] * scale;
        result[i].y = colors[i * 4 + 1] * scale;
        result[i].z = colors[i * 4 + 2] * scale;
        result[i].w = colors[i * 4 + 3] * scale;
    }

    return result;
}
*/