#include "Framework.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    :data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = dataSize;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    DEVICE->CreateBuffer(&desc, nullptr, &buffer);
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::Update()
{
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
    Update();
    DC->VSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
    Update();
    DC->PSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetCSBuffer(UINT slot)
{
    Update();
    DC->CSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetGSBuffer(UINT slot)
{
    Update();
    DC->GSSetConstantBuffers(slot, 1, &buffer);
}




/*
ConstBuffer::ConstBuffer(void* data, UINT dataSize)
    :data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;   // map�� ������ default -> dynamic
    desc.ByteWidth = dataSize;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   // �б�� ���ٰ���

    DEVICE->CreateBuffer(&desc, nullptr, &buffer);
}

* 
* void ConstBuffer::Update()
{
    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);// gpu�� �ִ� �޸𸮸� ���� �Լ�
    memcpy(subResource.pData, data, dataSize);                  // cpu�� �ִ� �޸𸮸� gpu�� �״�� ����
    DC->Unmap(buffer, 0);                                       // �ٸ� ������ ������ �� �ֵ��� �ݾ��ش�
}
*/
