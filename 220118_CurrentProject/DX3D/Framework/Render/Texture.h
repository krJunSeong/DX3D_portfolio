#pragma once

class Texture
{
private:
    wstring file;

    ScratchImage image;

    ID3D11ShaderResourceView* srv;

    static map<wstring, Texture*> textures;

    Texture(ID3D11ShaderResourceView* srv, ScratchImage& image);
    ~Texture();

public:
    static Texture* Add(wstring file);
    static Texture* Add(wstring file, wstring key);
    static void Delete();

    void Set(UINT slot);

    vector<Float4> ReadPixels();

    UINT Width() { return image.GetMetadata().width; }
    UINT Height() { return image.GetMetadata().height; }

    ID3D11ShaderResourceView*& GetSRV() { return srv; }

    wstring GetFile() { return file; }
};