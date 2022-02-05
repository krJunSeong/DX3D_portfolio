#pragma once

class Device : public Singleton<Device>
{
private:
    friend class Singleton;

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;

    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;

    Device();
    ~Device();

public:
    void CreateDeviceAndSwapChain();
    void CreateBackBuffer();

    void SetRenderTarget();
    void Clear();
    void Present();

    ID3D11Device* GetDevice() { return device; }
    ID3D11DeviceContext* GetDeviceContext() { return deviceContext; }
    IDXGISwapChain* GetSwapChain() { return swapChain; }
};