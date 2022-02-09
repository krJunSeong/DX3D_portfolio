#pragma once

class ConstBuffer
{
private:
    ID3D11Buffer* buffer;

    void* data;
    UINT dataSize;

    D3D11_MAPPED_SUBRESOURCE subResource;
protected:
    ConstBuffer(void* data, UINT dataSize);
    virtual ~ConstBuffer();

    void Update();

public:
    void SetVSBuffer(UINT slot);
    void SetPSBuffer(UINT slot);
    void SetCSBuffer(UINT slot);
    void SetGSBuffer(UINT slot);
};
/* 주의사항
	상수버퍼 넘겨줄 때 16 배수로 넘겨줄 것
	내가 선언한 곳에서만 쓸 수 있음.
		픽세루세이더에 선언하면 거기서만. 버텍스쉐이더에 선언하면 거기서만.
		물론 둘 다 선언가능함.
*/