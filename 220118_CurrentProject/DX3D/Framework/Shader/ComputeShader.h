#pragma once

class ComputeShader : public Shader
{
private:
    friend class Shader;

    ID3D11ComputeShader* shader;

    ComputeShader(wstring file);
    ~ComputeShader();

public:
    virtual void Set() override;
};