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
    : srv(srv), image(move(image))  // �̵�������
{
}

Texture::~Texture()
{
    srv->Release();
}

Texture* Texture::Add(wstring file)
{
    if(textures.count(file) > 0)
        return textures[file]; // ���� ������ �ִ� �� ��ȯ

    ScratchImage image;

    wstring extension = GetExtension(file);  // Ȯ���ڸ� �޾ƿð� LoadFromWICfile�� jpg ���� �͸� �аŵ�

    // ------------------------ Ȯ���� �д� �Լ� ����ó�� ------------------------
    if(extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if(extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(),WIC_FLAGS_FORCE_RGB, nullptr, image);


    ID3D11ShaderResourceView* srv;

    // �̹��� ��� �߸� �־��� �� ���⼭ ��Ʈ����.
    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    textures[file] = new Texture(srv, image);

    return textures[file];
}

Texture* Texture::Add(wstring file, wstring key)
{
    if (textures.count(key) > 0)
        return textures[key]; // ���� ������ �ִ� �� ��ȯ

    ScratchImage image;

    wstring extension = GetExtension(file);  // Ȯ���ڸ� �޾ƿð� LoadFromWICfile�� jpg ���� �͸� �аŵ�

    // ------------------------ Ȯ���� �д� �Լ� ����ó�� ------------------------
    if (extension == L"tga")
        LoadFromTGAFile(file.c_str(), nullptr, image);
    else if (extension == L"dds")
        LoadFromDDSFile(file.c_str(), DDS_FLAGS_NONE, nullptr, image);
    else
        LoadFromWICFile(file.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, image);


    ID3D11ShaderResourceView* srv;

    // �̹��� ��� �߸� �־��� �� ���⼭ ��Ʈ����.
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
    
    uint8_t* colors = image.GetPixels(); // �迭�� ����, �ϳ����� ��.
    UINT size = image.GetPixelsSize();  // �迭ũ��

    float scale = 1.0f / 255.0f; // ���� 0 ~ 255�� 0 ~ 1�� �ٲٴ� �۾�.

    // colors 4���� �𿩾� Float4�� ��
    // �÷��� ���������� ������� 4�� �������� �ȴ�.
    vector<Float4> result(size / 4);

    // Float[0...]��  rgba ���� �̾��ش�.
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