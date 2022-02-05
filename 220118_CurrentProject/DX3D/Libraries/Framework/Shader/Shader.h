#pragma once

class VertexShader;
class PixelShader;
class ComputeShader;

class Shader
{
private:
    static map<wstring, Shader*> shaders;

protected:
    wstring file;

    ID3DBlob* blob;

    virtual ~Shader() = default;

public:
    static VertexShader* AddVS(wstring file);
    static PixelShader* AddPS(wstring file);
    static ComputeShader* AddCS(wstring file);

    static void Delete();

    virtual void Set() = 0;

    wstring GetFile() { return file; }
};