#pragma once

class RawBuffer
{
private:
    ID3D11Resource* output;//GPU->output
    ID3D11UnorderedAccessView* uav;

    ID3D11Resource* result;//result->CPU

    UINT byteWidth;

public:
    RawBuffer(UINT byteWidth);
    ~RawBuffer();

    void Copy(void* data, UINT size);

    ID3D11UnorderedAccessView*& GetUAV() { return uav; }

private:
    void CreateOutput();
    void CreateUAV();
    void CreateResult();
};