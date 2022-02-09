#pragma once

class BlendState
{
private:
    D3D11_BLEND_DESC desc;
    ID3D11BlendState* state;

public:
    BlendState();
    ~BlendState();

    void SetState();

    void Alpha(bool value);
    void Additive();
    void AlphaToCoverage(bool value);

    void Changed();
};