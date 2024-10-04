// Directx1.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Directx1.h"
#include<D3D11.h>
#include<atlbase.h>
#include<D3DX11async.h>
#include <D3DX10math.h>

#define MAX_LOADSTRING 100

//#define HOUSE
#include<xnamath.h>
struct ConstantBuffer
{
    D3DMATRIX mWorld;
    //XMMATRIX mView;
    //XMMATRIX mProjection;
    //float x;
};
float dx = 0;
float dy = 0;


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
bool CREATED1 = false, CREATED2 = false;

CComPtr<IDXGISwapChain>         pSwapChain;
CComPtr<ID3D11DeviceContext>    pDeviceContext;
CComPtr<ID3D11Texture2D> pBackBuffer;
CComPtr<ID3D11RenderTargetView> pRenderTargetView;
CComPtr<ID3D11Device>           pd3dDevice;
CComPtr<ID3D11Buffer>           pVertexBuffer;
CComPtr<ID3D11Buffer>           pConstantBuffer;
CComPtr<ID3D11PixelShader>      pPixelShader, pPixelShader2;
CComPtr<ID3D11InputLayout>      pVertexLayout;
CComPtr<ID3D11VertexShader>     pVertexShader, pVertexShader2;
CComPtr<ID3D11DepthStencilView> pDepthStencilView;
CComPtr<ID3D11ShaderResourceView> texture1, texture2, texture3, texture4, texture5, texture6;
CComPtr<ID3D11SamplerState> g_pSamLinear;
CComPtr<ID3D11Texture2D> pDepthStencil;

CComPtr<ID3D11Texture2D> pEmptyTexture, pTempDepth;
CComPtr<ID3D11ShaderResourceView> pResourceViewForEmptyText, pDepthResourceView;
CComPtr<ID3D11RenderTargetView> pRenderTargetView2;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void OnIdle();


//struct pos {
//    float p[3];
//    float color[3];
//};
struct pos {
    float p[3];
    float t[2];
};

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
    LoadStringW(hInstance, IDC_DIRECTX1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX1));

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

    pVertexShader.Release();
    pPixelShader.Release();

    return 0;

    return (int)msg.wParam;
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DIRECTX1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

void DXCreateVertex()
{
    /*pos p[3] = { -.5, 0, 1, 1, 0, 0,
        0, -1, 1, 0, 1, 0,
        1, 1, 1, 0, 0, 1};*/
        /*pos p[8] = { -.5, .5, .1, 1,0,0,
            -.5, -.5, .1, 1,0,0,
            .5, .5, .1,1,0,0,
            .5, -.5, .1,1,0,0,*/
            /*
            -.5 , .5, -.1,0,1,0,
            -.5, -.5, -.1,0,1,0,
            .5, .5, -.1,0,1,0,
            .5, -.5, -.1,0,1,0*/
            //};
    pos p[36] = {

        -1, -1, 0, 1, 1,
        -1, 1, 0, 1, 0,
        1, -1, 0, 0, 1,
        1, 1, 0, 0, 0,

        0, 0, 0,    1, 1,
        0, .6, 0,  1, 0,
        .6, 0, 0,  0, 1,
        .6,.6, 0,  0, 0,

        0, 0, -0.2,  1, 1,
        0, .6, -0.2,  1, 0,
        .6, 0, -0.2,  0, 1,
        .6,.6, -0.2,  0, 0,

        /*.3,  .6,    0,   1,0,
        .3,   0,    0,   1,1,
        .3,  .6,   .6,   0,0,
        .3,   0,   .6,   0,1,

       /*-.3,  .3, -.3,   1,0,
       -.3, -.3, -.3,   1,1,
        .3,  .3, -.3,   0,0,
        .3, -.3, -.3,   0,1,*/

        /*.3,  .3,  .3,   1,0,
        .3, -.3,  .3,   1,1,
        .3,  .3, -.3,   0,0,
        .3, -.3, -.3,   0,1,*/

       -.3,  .3,  .3,   1,0,
       -.3, -.3,  .3,   1,1,
       -.3,  .3, -.3,   0,0,
       -.3, -.3, -.3,   0,1,

        .3, -.3,  .3,   1,0,
       -.3, -.3,  .3,   1,1,
        .3, -.3, -.3,   0,0,
       -.3, -.3, -.3,   0,1,

        .3,  .3,  .3,   0,0,
       -.3,  .3,  .3,   1,0,
        .3,  .3, -.3,   0,1,
       -.3,  .3, -.3,   1,1,

       -.3,  .3, -.3,  1,0,
        .3,  .3, -.3,  0,1,
         0,  .8,   0,  1,1,

       -.3,  .3, -.3,  1,0,
       -.3,  .3,  .3,  0,1,
         0,  .8,   0,  1,1,

       -.3,  .3,  .3,  1,0,
        .3,  .3,  .3,  0,1,
         0,  .8,   0,  1,1,

        .3,  .3, -.3,  1,0,
        .3,  .3,  .3,  0,1,
         0,  .8,   0,  1,1,
    };

    /*pos top[12] = {
          -.3, -.3, -.3,  1,0,
           .3, -.3, -.3,  0,1,
            0, -.6,   0,  1,1

          -.3, -.3, -.3,  1,0,
          -.3, -.3,  .3,  0,1,
            0, -.6,   0,  1,1,

          -.3, -.3,  .3,  1,0,
           .3, -.3,  .3,  0,1,
            0, -.6,   0,  1,1,

           .3, -.3, -.3,  1,0,
           .3, -.3,  .3,  0,1,
            0, -.6,   0,  1,1,
    };*/

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(p);
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = p;

    HRESULT hr = pd3dDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
        {"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "VS", "vs_5_0", &pVSBlob);
        hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader);

        // Create the input layout
        hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
        //MessageBoxA(0, "check layout", "", 0);
    }

    {
        CComPtr<ID3DBlob> pVSBlob = NULL;
        hr = CompileShaderFromFile("effect.fx", "VS2", "vs_5_0", &pVSBlob);
        hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &pVertexShader2);

        // Create the input layout
        //hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &pVertexLayout);
        //MessageBoxA(0, "check layout", "", 0);
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
    // Create the constant buffer
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = pd3dDevice->CreateBuffer(&bd, NULL, &pConstantBuffer);
}

void DXCreateDevice(HWND g_hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;

    sd.BufferDesc.Width = 640;
    sd.BufferDesc.Height = 480;

    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
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
    ID3D11RenderTargetView* p[] = { pRenderTargetView.p };
    hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
    //pDeviceContext->OMSetRenderTargets(1, p, pDepthStencilView);

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


#ifndef HOUSE
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text1.jpg", NULL, NULL, &texture1, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text2.jpg", NULL, NULL, &texture2, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text3.jpg", NULL, NULL, &texture3, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text4.jpg", NULL, NULL, &texture4, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text5.jpg", NULL, NULL, &texture5, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text6.jpg", NULL, NULL, &texture6, &hr);
#endif
#ifdef HOUSE
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\bodyback.bmp", NULL, NULL, &texture1, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\bodyfront.bmp", NULL, NULL, &texture2, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\bodyright.bmp", NULL, NULL, &texture3, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\bodyleft.bmp", NULL, NULL, &texture4, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\text5.jpg", NULL, NULL, &texture5, &hr);
    hr = D3DX11CreateShaderResourceViewFromFile(pd3dDevice, "C:\\Users\\K Raviraju Prasanna\\Desktop\\texture.bmp", NULL, NULL, &texture6, &hr);
#endif
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

    D3D11_TEXTURE2D_DESC desc = {};
    pBackBuffer->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    hr = pd3dDevice->CreateTexture2D(&desc, 0, &pEmptyTexture);
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC rvdesc = {};
        rvdesc.Format = desc.Format;
        rvdesc.ViewDimension = D3D10_1_SRV_DIMENSION_TEXTURE2D;
        rvdesc.Texture2D.MipLevels = 1;
        rvdesc.Texture2D.MostDetailedMip = 0;

        hr = pd3dDevice->CreateShaderResourceView(pEmptyTexture, &rvdesc, &pResourceViewForEmptyText);
        hr = pd3dDevice->CreateRenderTargetView(pEmptyTexture, NULL, &pRenderTargetView2);
    }
    {
        //CComPtr<ID3D11Texture2D> drawBuf;
        D3D11_TEXTURE2D_DESC rvdesc = {};


        pDepthStencil->GetDesc(&rvdesc);
        rvdesc.Format = DXGI_FORMAT_R32_TYPELESS;//desc.Format;
        rvdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        hr = pd3dDevice->CreateTexture2D(&rvdesc, 0, &pTempDepth);



        D3D11_SHADER_RESOURCE_VIEW_DESC depth_rvdesc = {};
        depth_rvdesc.Format = DXGI_FORMAT_R32_SINT;
        depth_rvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        depth_rvdesc.Texture2D.MipLevels = 1;
        depth_rvdesc.Texture2D.MostDetailedMip = 0;
        hr = pd3dDevice->CreateShaderResourceView(pTempDepth, &depth_rvdesc, &pDepthResourceView);
        //hr = pd3dDevice->CreateShaderResourceView(pDepthStencil, &rvdesc, &pDepthResourceView);
    }
    
}


bool Func(HWND g_hWnd);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 640, 480, nullptr, nullptr, hInstance, nullptr);

    SetTimer(hWnd,             // handle to main window 
        501,            // timer identifier 
        2000,                 // 10-second interval 
        (TIMERPROC)NULL);     // no timer callback 

    if (!hWnd)
    {
        return FALSE;
    }
    //Func(hWnd);
    DXCreateDevice(hWnd);
    DXCreateVertex();
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void OnIdle()
{
    //set sampler and texture
    pDeviceContext->PSSetSamplers(0, 1, &g_pSamLinear.p);

    float ClearColor[4] = { 0.0f, 0.125f, 0.6f, 1.0f }; // RGBA
    float ClearColor2[4] = { 0.6f, 0.125f, 0.0f, 1.0f };
    pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
    pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Set vertex buffer
    UINT stride = sizeof(pos);
    UINT offset = 0;
    pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer.p, &stride, &offset);
    pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    pDeviceContext->IASetInputLayout(pVertexLayout);


    //update the matrix
    ConstantBuffer cb;
    static float t = 0;
    t = t + .001;
    if (t >= 6.3)t = 0;
    //cb.x = t;
#ifdef HOUSE
    cb.mWorld = XMMatrixRotationY(t);
#endif
    //cb.mWorld = XMMatrixRotationY(CUt);
    //cb.mWorld = XMMatrixIdentity();
#ifndef HOUSE
    //FXMVECTOR f = XMVectorSet(1, 1, 1, 0);
    //cb.mWorld = XMMatrixRotationAxis(f, t);
    //cb.mWorld = XMMatrixTranslation(-.3, -.3, 0) * XMMatrixRotationY(t) * XMMatrixTranslation(.3, .3, 0);
#endif
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);

    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 vEyePt(0.0f, 0.1f, -0.4f);
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);

    D3DXMATRIX worldMatrix; //D3DXMatrixIdentity(&worldMatrix);
    D3DXMatrixRotationY(&worldMatrix, t);
    //matView = worldMatrix * matView;
    cb.mWorld = worldMatrix;

    pDeviceContext->UpdateSubresource(pConstantBuffer, 0, NULL, &cb, 0, 0);

    // Render a triangle 
    pDeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer.p);
    pDeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer.p);


    //ID3D11ShaderResourceView* text[] = { texture1.p, texture2.p };

    pDeviceContext->ClearRenderTargetView(pRenderTargetView2, ClearColor2);

    //NOTE : use pixelShader2 for the blur effect, and pixelShader for no effects
    {
        pDeviceContext->VSSetShader(pVertexShader, NULL, 0);
        pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
        pDeviceContext->PSSetShaderResources(0, 1, &texture1.p);
        ID3D11RenderTargetView* p[] = { pRenderTargetView2.p };
        pDeviceContext->OMSetRenderTargets(1, p, pDepthStencilView);

        pDeviceContext->Draw(4, 8);
        pDeviceContext->PSSetShaderResources(0, 1, &texture2.p);
        pDeviceContext->Draw(4, 4);
    }

    {
        pDeviceContext->VSSetShader(pVertexShader2, NULL, 0);
        pDeviceContext->PSSetShader(pPixelShader, NULL, 0);
        ID3D11RenderTargetView* p[] = { pRenderTargetView.p };
        pDeviceContext->OMSetRenderTargets(1, p, NULL);

        pDeviceContext->CopyResource(pTempDepth, pDepthStencil);
        pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor2);
        pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        pDeviceContext->PSSetShaderResources(0, 1, &pResourceViewForEmptyText.p);
        //pDeviceContext->PSSetShaderResources(0, 1, &pDepthResourceView.p);
        pDeviceContext->Draw(4, 4);
    }

    //HRESULT hr = D3DX11SaveTextureToFileA(pDeviceContext, pBackBuffer, D3DX11_IFF_BMP, "C:\\Users\\K Raviraju Prasanna\\Desktop\\textn.bmp");
    //hr = D3DX11SaveTextureToFileA(pDeviceContext, pEmptyTexture, D3DX11_IFF_BMP, "C:\\Users\\K Raviraju Prasanna\\Desktop\\textn2.bmp");
    /*
    pDeviceContext->PSSetShaderResources(0, 1, &texture2.p);
    pDeviceContext->Draw(4, 4);

    pDeviceContext->PSSetShaderResources(0, 1, &texture3.p);
    pDeviceContext->Draw(4, 8);

    pDeviceContext->PSSetShaderResources(0, 1, &texture4.p);
    pDeviceContext->Draw(4, 12);

    pDeviceContext->PSSetShaderResources(0, 1, &texture5.p);
    pDeviceContext->Draw(4, 16);

    pDeviceContext->PSSetShaderResources(0, 1, &texture6.p);
    pDeviceContext->Draw(4, 20);
    */
#ifdef HOUSE
    pDeviceContext->Draw(3, 24);

    pDeviceContext->Draw(3, 27);

    pDeviceContext->Draw(3, 30);

    pDeviceContext->Draw(3, 33);
#endif
    pSwapChain->Present(0, 0);
}

//Function:  func
// DirectX
bool Func(HWND g_hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 640;
    sd.BufferDesc.Height = 480;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;




    D3D_FEATURE_LEVEL uFeatureLevel[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    D3D_FEATURE_LEVEL uFeatureLevel1;
    HRESULT hr;


    /*float r = (rand() % 10) / 9.0;
    float g = (rand() % 10) / 9.0;
    float b = (rand() % 10) / 9.0;
    float ClearColor[4] = { r, g, b, 1.0f }; //RGBA
    */

    //if (!CREATED2)

        //CREATED2 = true;
    hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, 0, uFeatureLevel, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &uFeatureLevel1, &pDeviceContext);
    hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    hr = pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
    //pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView.p, 0);

    CComPtr<ID3D11Texture2D> pDepthStencil;
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


    /*ID3D11RenderTargetView* p[] = { pRenderTargetView.p };
    hr = pd3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);
    pDeviceContext->OMSetRenderTargets(1, p, pDepthStencilView);*/

    D3D11_VIEWPORT vp = {};
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width = sd.BufferDesc.Width;
    vp.Height = sd.BufferDesc.Height;
    pDeviceContext->RSSetViewports(1, &vp);

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

    //pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
    //pSwapChain->Present(0, 0); //this will swap buffer 

    //pSwapChain->Present(0, 0);
    return true;
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
    //Func(hWnd);

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
        case ID_START:
            //Func(hWnd);
            OnIdle();
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
    case WM_KEYDOWN:
        if (VK_LEFT == wParam)dx = max(dx - 0.1, -1);
        if (VK_RIGHT == wParam)dx = min(dx + 0.1, 1);
        if (VK_UP == wParam)dy = min(dy + 0.1, 1);
        if (VK_DOWN == wParam)dy = max(dy - 0.1, -1);
        if (VK_SPACE == wParam)dx = dy = 0;
        break;
    case WM_TIMER:

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
