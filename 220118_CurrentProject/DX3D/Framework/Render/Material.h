#pragma once

class Material
{
private:
    string name;
    string editName;
    string file;

    VertexShader* vertexShader;
    PixelShader* pixelShader;

    Texture* diffuseMap;
    Texture* specularMap;
    Texture* normalMap;

    MaterialBuffer* buffer;
public:
    Material();
    Material(wstring shaderFile);
    ~Material();

    void Set();

    void GUIRender();

    void SetShader(wstring file);

    void Save(string file);
    void Load(string file);

    void SetDiffuseMap(wstring file);
    void SetSpecularMap(wstring file);
    void SetNormalMap(wstring file);

    void SetName(string name) { this->name = name; }

    Texture* GetDiffuseMap() { return diffuseMap; }
    Texture* GetSpecularMap() { return specularMap; }
    Texture* GetNormalMap() { return normalMap; }

    MaterialBuffer::Data& GetData() { return buffer->data; }

    string GetName() { return name; }

    bool HasShader() { return (vertexShader != nullptr) ? true : false; }
private:
    void SelectShader();

    void SelectMap(string name, int type);
    void UnselectMap(int type);

    void SaveDialog();
    void LoadDialog();
};