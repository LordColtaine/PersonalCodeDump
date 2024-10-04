#pragma once

#include<D3D11.h>
#include<atlbase.h>
struct DeviceAndSwapChain
{
	CComPtr<IDXGISwapChain> pSwapChain;
	CComPtr<ID3D11Device>   pDevice;
	CComPtr<ID3D11DeviceContext> pDeviceContext;
	CComPtr<ID3D11Texture2D>          pBackBuffer;

	DeviceAndSwapChain(HWND g_hWnd);

};

struct BackBuffer
{
	CComPtr<ID3D11Texture2D> pBackBuffer;
	CComPtr<ID3D11ShaderResourceView> pResourceView;
	
	BackBuffer(DeviceAndSwapChain);
};

struct TargetView
{
	CComPtr<ID3D11RenderTargetView> pRenderTargetView;
	TargetView(DeviceAndSwapChain, BackBuffer);
};