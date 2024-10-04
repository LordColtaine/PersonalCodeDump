// DirectX2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DirectX2.h"
#include <D3D11.h>
#include<atlbase.h>
#include<D3DX11async.h>
#include<windowsx.h>
#include <D3DX10math.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CComPtr<IDXGISwapChain>           pSwapChain;
CComPtr<ID3D11DeviceContext>      pDeviceContext;
CComPtr<ID3D11Texture2D>          pBackBuffer;
CComPtr<ID3D11Device>             pd3dDevice;
CComPtr<ID3D11Texture2D>          pDepthStencil;
CComPtr<ID3D11RenderTargetView>   pRenderTargetView;
CComPtr<ID3D11DepthStencilView>   pDepthStencilView;
CComPtr<ID3D11SamplerState>       g_pSamLinear;

CComPtr<ID3D11Texture2D>          pFrameBackBuffer, pTempDepth;
CComPtr<ID3D11ShaderResourceView> pFrameResourceView, ptempDepthRV;
CComPtr<ID3D11RenderTargetView>   pFrameRenderTargetView;
CComPtr<ID3D11Buffer>             pVertexBuffer;
CComPtr<ID3D11Buffer>             pConstantBuffer;
CComPtr<ID3D11PixelShader>        pPixelShader, pPixelShader2;
CComPtr<ID3D11InputLayout>        pVertexLayout;
CComPtr<ID3D11VertexShader>       pVertexShader, pVertexShader2;

struct ConstantBuffer
{
    D3DXMATRIX mWorld;
};

struct Position
{
    float p[3];
    float t[2];
};
CComPtr<ID3D11ShaderResourceView> sky;
CComPtr<ID3D11ShaderResourceView> ground;


float dxPos = 0, dyPos = 0, dxAt = 0, dyAt = 0;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                OnIdle();
void                CreateDevice(HWND hwnd);
void                CreateShaders();
void                X();

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
    LoadStringW(hInstance, IDC_DIRECTX2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX2));

    MSG msg = {};

    // Main message loop:
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            OnIdle();
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECTX2);
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
   CreateDevice(hWnd);
   CreateShaders();
   X();
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
    case WM_KEYDOWN:
        if (VK_LEFT == wParam)
            dxPos = max(-1, dxPos - 0.01);
        if (VK_RIGHT == wParam)
            dxPos = min(1, dxPos + 0.01);
        if (VK_UP == wParam)
            dyPos = min(1, dyPos + 0.01);
        if (VK_DOWN == wParam)
            dyPos = max(-1, dyPos - 0.01);
        if (VK_SPACE == wParam)
            dyPos = dxPos = dxAt = dyAt = 0;
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
//Function that creates all the necessary devices and contexts
void CreateDevice(HWND g_hWnd)
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.BufferDesc.Width = 640;
    SwapChainDesc.BufferDesc.Height = 480;

    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = g_hWnd;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL uFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0, };
    D3D_FEATURE_LEVEL uFeatureLevel1;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, uFeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDesc, &pSwapChain, &pd3dDevice, &uFeatureLevel1, &pDeviceContext);
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = SwapChainDesc.BufferDesc.Width;
    descDepth.Height = SwapChainDesc.BufferDesc.Height;
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

    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    descDSV.Flags = 0;

    hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
    ID3D11RenderTargetView* p[] = { pRenderTargetView.p };
    hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);

    D3D11_VIEWPORT vp = {};
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = SwapChainDesc.BufferDesc.Width;
    vp.Height = SwapChainDesc.BufferDesc.Height;
    pDeviceContext->RSSetViewports(1, &vp);

    //disable culling
    D3D11_RASTERIZER_DESC rasterizerState = {};
    rasterizerState.FillMode = D3D11_FILL_SOLID;//D3D11_FILL_WIREFRAME;
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
    /*
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
    pd3dDevice->CreateSamplerState(&SamDesc, &g_pSamLinear);*/

    D3D11_TEXTURE2D_DESC desc = {};
    pBackBuffer->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    hr = pd3dDevice->CreateTexture2D(&desc, 0, &pFrameBackBuffer);
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = {};
        rvdesc.Format = desc.Format;
        rvdesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
        rvdesc.Texture2D.MipLevels = 1;
        rvdesc.Texture2D.MostDetailedMip = 0;

        hr = pd3dDevice->CreateShaderResourceView(pFrameBackBuffer, &rvdesc, &pFrameResourceView);
        hr = pd3dDevice->CreateRenderTargetView(pFrameBackBuffer, NULL, &pFrameRenderTargetView);
    }

    //creating offscreen depth buffer
    D3D11_TEXTURE2D_DESC rvdesc = {};
    pDepthStencil->GetDesc(&rvdesc);
    rvdesc.Format = DXGI_FORMAT_R32_TYPELESS;
    rvdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    hr = pd3dDevice->CreateTexture2D(&rvdesc, 0, &pTempDepth);

    D3D11_SHADER_RESOURCE_VIEW_DESC depth_rvdesc = {};
    depth_rvdesc.Format = DXGI_FORMAT_R32_SINT;
    depth_rvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    depth_rvdesc.Texture2D.MipLevels = 1;
    depth_rvdesc.Texture2D.MostDetailedMip = 0;
    hr = pd3dDevice->CreateShaderResourceView(pTempDepth, &depth_rvdesc, &ptempDepthRV);

    D3D11_BUFFER_DESC bd = {};
    bd.MiscFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);

    D3D11_BLEND_DESC BlendState;
    ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
    BlendState.RenderTarget[0].BlendEnable = TRUE;
    BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    BlendState.RenderTarget[0].RenderTargetWriteMask = 0x0f;
    float f[4] = { 0, 0, 0, 0 };
    CComPtr<ID3D11BlendState> pBlendState;
    hr = pd3dDevice->CreateBlendState(&BlendState, &pBlendState);
    //pDeviceContext->OMSetBlendState(pBlendState, f, 0xffffffff);

}

HRESULT CompileShaderFromFile(const char* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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

void CreateShaders()
{
    HRESULT hr;

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "VS", "vs_5_0", &pVSBlob);
        hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);

        // Create the input layout
        hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
    }

    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "VS2", "vs_5_0", &pVSBlob);
        hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader2);
    }
    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "PS", "ps_5_0", &pVSBlob);
        hr = pd3dDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pPixelShader);
    }

    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "PS2", "ps_5_0", &pVSBlob);
        hr = pd3dDevice->CreatePixelShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pPixelShader2);
    }




    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "textures\\ground.bmp", NULL, NULL, &ground, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "textures\\sky.bmp", NULL, NULL, &sky, &hr);

}

void X()
{
    Position p[12] = {
        //rect 1
        0, 0, 0,    1, 1,
        0, .6, 0,  1, 0,
        .6, 0, 0,  0, 1,
        .6,.6, 0,  0, 0,

        //rect2
        0, 0, -0.3,  1, 1,
        0, .6, -0.3,  1, 0,
        .6, 0, -0.3,  0, 1,
        .6,.6, -0.3,  0, 0,

        -1, -1, 0, 0, 0,
        -1, 1, 0, 0, 1,
        1, -1, 0, 1, 0,
        1, 1, 0, 1, 1,
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
}
//Function that is executed when no action is taking place
void OnIdle()
{
    //set sampler and texture
    //pDeviceContext->PSSetSamplers(0, 1, &g_pSamLinear.p);

    float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA
    float ClearColor2[4] = { 0.6f, 0.125f, 0.0f, 1.0f };
    pDeviceContext->ClearRenderTargetView(pFrameRenderTargetView, ClearColor2);
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor2);
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set vertex buffer
    UINT stride = sizeof(Position);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer.p, &stride, &offset);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    pDeviceContext->IASetInputLayout(pVertexLayout);

    static float t = 0;
    t = t + .001;
    if (t >= 6.28)t = 0;

    //update the matrix
    ConstantBuffer cb;
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

    D3DXVECTOR3 vEyePt(0.0f+dxPos, 0.8f+dyPos, -1.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
    D3DXMATRIX viewMatrix;
    D3DXMatrixLookAtLH(&viewMatrix, &vEyePt, &vLookatPt, &vUpVec);

    D3DXMATRIX worldMatrix, toOrigin, toOG;
    //D3DXMatrixIdentity(&worldMatrix);
    D3DXMatrixRotationY(&worldMatrix, t);
    //D3DXMatrixTranslation(&toOrigin, -.3, -.3, 0);

    //D3DXMatrixTranslation(&toOG, +.3, +.3, 0);
    //worldMatrix = toOrigin * worldMatrix * toOG;
    //D3DXMatrixIdentity(&worldMatrix);
    viewMatrix = worldMatrix * viewMatrix *matProj;
    cb.mWorld = viewMatrix;
    cb.mWorld = worldMatrix;

    pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);

    // Render a triangle 
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer.p);
    pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer.p);


    //ID3D11ShaderResourceView* text[] = { texture1.p, texture2.p };

    //pDeviceContext->ClearRenderTargetView(pRenderTargetView2, ClearColor2);

    //NOTE : use pixelShader2 for the blur effect, and pixelShader for no effects
    
    {
        pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
        pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
        pDeviceContext->PSSetShaderResources(0, 1, &sky.p);
        ID3D11RenderTargetView* p[] = { pFrameRenderTargetView.p };
        pDeviceContext->OMSetRenderTargets(1, p, pDepthStencilView);

        pDeviceContext->Draw(4, 0);
        pDeviceContext->PSSetShaderResources(0, 1, &ground.p);
        pDeviceContext->Draw(4, 4);
    }

    {
        pDeviceContext->CopyResource(pTempDepth, pDepthStencil);
        pDeviceContext->VSSetShader(pVertexShader2, NULL, 0);
        pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
        ID3D11RenderTargetView* p[] = { pRenderTargetView.p };
        pDeviceContext->OMSetRenderTargets(1, p, NULL);
        //pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
        //pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        ID3D11ShaderResourceView* t[] = {pFrameResourceView, ptempDepthRV};
        pDeviceContext->PSSetShaderResources(0, 2, t);
        pDeviceContext->PSSetShaderResources(0, 1, &ptempDepthRV.p);
        pDeviceContext->Draw(4, 8);
        
    }
    pSwapChain->Present(0, 0);

}
