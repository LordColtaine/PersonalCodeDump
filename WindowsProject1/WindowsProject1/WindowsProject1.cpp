// WindowsProject1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "WindowsProject1.h"

#include<atlbase.h>
#include<D3D11.h>
#include<Windows.h>
#include<D3DX11async.h>
#include<xnamath.h>
#include<D3dx9math.h>


#define MAX_LOADSTRING 100
//bool func(HWND hWnd);
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
CComPtr<IDXGISwapChain>         pSwapChain;
CComPtr<ID3D11DeviceContext>    pDeviceContext;
CComPtr<ID3D11Texture2D> pBackBuffer;
CComPtr<ID3D11RenderTargetView> pRenderTargetView;
CComPtr<ID3D11Device>           pd3dDevice;
CComPtr<ID3D11Buffer>           pVertexBuffer;
CComPtr<ID3D11Buffer>           pVertexBuffer2;
CComPtr<ID3D11VertexShader>     pVertexShader;
CComPtr<ID3D11PixelShader>      pPixelShader;
CComPtr<ID3D11PixelShader>      pPixelShader0;
CComPtr<ID3D11InputLayout>      pVertexLayout;
CComPtr<ID3D11Buffer>           pConstantBuffer;
CComPtr<ID3D11DepthStencilView> pDepthStencilView;
CComPtr<ID3D11Texture2D> pDepthStencil;

CComPtr<ID3D11DepthStencilView> pDepthStencilView2;
CComPtr<ID3D11Texture2D> pDepthStencil2;
CComPtr<ID3D11ShaderResourceView> texture1;
CComPtr<ID3D11ShaderResourceView> texture2;
CComPtr<ID3D11ShaderResourceView> texture3;
CComPtr<ID3D11ShaderResourceView> texture4;
CComPtr<ID3D11ShaderResourceView> updated_texture;

CComPtr<ID3D11SamplerState> g_pSamLinear;
CComPtr<ID3D11RenderTargetView> pRenderTargetView2;
CComPtr<ID3D11ShaderResourceView> pDrawBuffer1_rv;
CComPtr<ID3D11Texture2D> pDrawBuffer1;
CComPtr<ID3D11RenderTargetView> pRenderTargetView1;




float counter = 0;
bool onIdleFirst = true;
/*
struct pos {
	float p[3];
	float color[3];
};
*/
struct pos {
	float p[3];
	float t[2];
	float l;

};
struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	//bool blur;
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

static float t = 0;


void OnIdle()
{

	onIdleFirst = false;
	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA
	float ClearColor1[4] = { 0.0f, 1.0f, 0.f, 1.0f }; // RGBA

	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	UINT stride = sizeof(pos);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer.p, &stride, &offset);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	pDeviceContext->IASetInputLayout(pVertexLayout);
	ConstantBuffer cb;
	XMMATRIX t1 = XMMatrixTranslation(.05, .25, .5);
	XMMATRIX r = XMMatrixIdentity();
	XMMATRIX t2 = XMMatrixTranslation(-.05, -.25, -.5);
	XMMATRIX m = t1 * r * t2;
	cb.mWorld = XMMatrixTranspose(m);
	cb.mView = XMMatrixIdentity();
	cb.mProjection = cb.mView;
	//cb.blur = false;
	pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);


	pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer.p);
	pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer.p);
	pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader0, NULL, 0);

	pDeviceContext->PSSetShaderResources(0, 1, &texture1.p);

	pDeviceContext->ClearRenderTargetView(pRenderTargetView1, ClearColor);
	{
		ID3D11RenderTargetView* p[] = { pRenderTargetView1.p, };
		pDeviceContext->OMSetRenderTargets(1, p, 0);
		pDeviceContext->Draw(4, 0);
	}
	pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
	pDeviceContext->PSSetShaderResources(1, 1, &pDrawBuffer1_rv.p);
	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor1);
	{
		ID3D11RenderTargetView* p[] = { pRenderTargetView.p, };
		pDeviceContext->OMSetRenderTargets(1, p, 0);
		pDeviceContext->Draw(4, 0);
	}



	//HRESULT hr = D3DX11SaveTextureToFileA(pDeviceContext, pDrawBuffer1, D3DX11_IFF_BMP, "a.bmp");


	//	hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, L"a.bmp", NULL, NULL, &updated_texture, &hr);
	//D3DX11CreateShaderResourceViewFromMemory(pd3dDevice,pDrawBuffer1,sizeof(pDrawBuffer1),NULL,NULL,&updated_texture,&hr);
	//D3DX11CreateTextureFromResourceA(pd3dDevice,)
	//D3DX11LoadTextureFromTexture(pDeviceContext, pDrawBuffer1, NULL, &updated_texture);

	//pDeviceContext->PSSetShaderResources(4, 1, &updated_texture.p);

	pSwapChain->Present(0, 0);
}

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
	ID3DBlob* pErrorBlob;

	hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel, dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			MessageBoxA(0, (char*)pErrorBlob->GetBufferPointer(), "Error", 0);
		if (pErrorBlob) pErrorBlob->Release();
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void DXCreateVertex()
{
	/*
	pos p[8] =
	{   -.5, -.5,- 0.5,           0, 1, 0,
		-.5, .5, -0.5,            0, 1, 0,
		.5, -.5, -0.5,            0, 1, 0,
		.5, .5, -0.5,             0, 1, 0,

		-.5, -.5, 0.5,           1, 0, 0,
		-.5, .5, 0.5,            1, 0, 0,
		 .5, -.5, 0.5,           1, 0, 0,
		 .5, .5, 0.5,            1, 0, 0,


	};
	*/

	/*pos p[16] = {
		-.5, -.5,-.5,             0, 1,		0,
		-.5, .5, -0.5,            0, 0,		0,
		.5, -.5, -0.5,            1, 1,		0,
		.5, .5, -0.5,             1, 0,		0,

	   -.5, -.5, .5,             0, 1,		1,
	   -.5,  .5, .5,             0, 0,		1,
		.5, -.5, .5,             1, 1,		1,
		.5,  .5, .5,             1, 0,		1,

		.5, -.5,-.5,			 0, 1,		2,
		.5, .5, -.5,			 0, 0,		2,
		.5, -.5, .5,			 1, 1,		2,
		.5, .5,  .5,			 1, 0,		2,

		-.5, -.5,-.5,             1, 1,		3,
		-.5,  .5, -.5,            1, 0,		3,
		-.5, -.5, .5,             0, 1,		3,
		-.5,  .5, .5,             0, 0,		3,


	};
	*/
	pos p[8] = {
		 -.5, -.5,-.5,            0, 1,		0,
		-.5, .5, -.5,             0, 0,		0,
		 .5, -.5, -.5,            1, 1,		0,
		 .5, .5, -.5,             1, 0,		0,


		 -.5, -.5,1,            0, 1,		0,
		-.5, .5, 1,             0, 0,		0,
		 .5, -.5, 1,            1, 1,		0,
		 .5, .5, 1,             1, 0,		0,


	};
	pos p2[4] = {
		-.5, -.5,-.5,             0, 1,		0,
		-.5, .5, -.5,             0, 0,		0,
		 .5, -.5, -.5,            1, 1,		0,
		 .5, .5, -.5,             1, 0,		0,

	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(p);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = p;

	HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);

	//D3D11_BUFFER_DESC bd2 = {};
	bd.ByteWidth = sizeof(p2);
	InitData = {};
	InitData.pSysMem = p2;

	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer2);

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION_ASIFD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{"POSITION_CAR",0,DXGI_FORMAT_R32G32B32_FLOAT,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//("TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0),
		{"TEXTURE",0,DXGI_FORMAT_R32G32_FLOAT,0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"LOCATION",0,DXGI_FORMAT_R32_FLOAT,0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},

	};
	UINT numElements = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	{
		CComPtr<ID3DBlob> pVSBlob = NULL;
		hr = CompileShaderFromFile(L"effect.fx", "VS", "vs_5_0", &pVSBlob);
		hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);

		// Create the input layout
		hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
		//MessageBoxA(0, "check layout", "", 0);
	}

	{
		CComPtr<ID3DBlob> pVSBlob = NULL;
		hr = CompileShaderFromFile(L"effect.fx", "PS", "ps_5_0", &pVSBlob);
		hr = pd3dDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pPixelShader);
		pVSBlob = NULL;
		hr = CompileShaderFromFile(L"effect.fx", "PS0", "ps_5_0", &pVSBlob);
		hr = pd3dDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pPixelShader0);

	}

	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);
}

void DXCreateDevice(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;

	sd.BufferDesc.Width = 500;
	sd.BufferDesc.Height = 500;

	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	//CComPtr<ID3D11Texture2D> pBackBuffer;

	D3D_FEATURE_LEVEL uFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0, };
	D3D_FEATURE_LEVEL uFeatureLevel1;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, 0, uFeatureLevel, 1, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &uFeatureLevel1, &pDeviceContext);
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = sd.BufferDesc.Width;
	descDepth.Height = sd.BufferDesc.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = pd3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);

	//
	//D3D11_TEXTURE2D_DESC desc = { ???? };????;
	D3D11_TEXTURE2D_DESC desc = {};
	pBackBuffer->GetDesc(&desc);
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;;

	hr = pd3dDevice->CreateTexture2D(&desc, 0, &pDrawBuffer1);
	//
	//D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = { ???? }????;
	D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = {};
	rvdesc.Format = desc.Format;
	rvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rvdesc.Texture2D.MipLevels = 1; //will not allow mipmapping
	rvdesc.Texture2D.MostDetailedMip = 0;

	hr = pd3dDevice->CreateShaderResourceView(pDrawBuffer1, &rvdesc, &pDrawBuffer1_rv);
	hr = pd3dDevice->CreateRenderTargetView(pDrawBuffer1, NULL, &pRenderTargetView1);

	D3D11_VIEWPORT vp = {};
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = sd.BufferDesc.Width;
	vp.Height = sd.BufferDesc.Height;
	pDeviceContext->RSSetViewports(1, &vp);

	//disable culling
	D3D11_RASTERIZER_DESC rasterizerState = {};
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	rasterizerState.FrontCounterClockwise = false;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 1;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = false;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = true;

	CComPtr<ID3D11RasterizerState> pRasterizerState = 0;
	pd3dDevice->CreateRasterizerState(&rasterizerState, &pRasterizerState);
	pDeviceContext->RSSetState(pRasterizerState);


	hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, L"poi_root.bmp", NULL, NULL, &texture1, &hr);
	//hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, L"poi_root2.bmp", NULL, NULL, &texture2, &hr);
	//hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, L"poi_root3.bmp", NULL, NULL, &texture3, &hr);
	//hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, L"poi_root4.bmp", NULL, NULL, &texture4, &hr);
	//pDeviceContext->PSSetShaderResources(0, 1, &texture1.p);
	//pDeviceContext->PSSetShaderResources(1, 1, &texture2.p);
	//pDeviceContext->PSSetShaderResources(2, 1, &texture3.p);
	//pDeviceContext->PSSetShaderResources(3, 1, &texture4.p);



	D3D11_SAMPLER_DESC SamDesc;
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	pd3dDevice->CreateSamplerState(&SamDesc, &g_pSamLinear);
	pDeviceContext->PSSetSamplers(0, 1, &g_pSamLinear.p);
	//pEffect->GetVariableByName("g_samLinear")->AsSampler()->SetSampler(0, g_pSamLinear);

}
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
