#include "Devices.h"

DeviceAndSwapChain::DeviceAndSwapChain(HWND h)
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = 640;
    SwapChainDesc.BufferDesc.Height = 480;

    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = h;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL uFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0, };
    D3D_FEATURE_LEVEL uFeatureLevel1;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, uFeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pDevice, &uFeatureLevel1, &pDeviceContext);
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
}

BackBuffer::BackBuffer(DeviceAndSwapChain d)
{
    D3D11_TEXTURE2D_DESC desc = {};
    d.pBackBuffer->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    HRESULT hr = d.pDevice->CreateTexture2D(&desc, 0, &pBackBuffer);
    D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = {};
    rvdesc.Format = desc.Format;
    rvdesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
    rvdesc.Texture2D.MipLevels = 1;
    rvdesc.Texture2D.MostDetailedMip = 0;

    hr = d.pDevice->CreateShaderResourceView(pBackBuffer, &rvdesc, &pResourceView);
}

TargetView::TargetView(DeviceAndSwapChain d, BackBuffer b)
{
    HRESULT hr = d.pDevice->CreateRenderTargetView(b.pBackBuffer, NULL, &pRenderTargetView);
}