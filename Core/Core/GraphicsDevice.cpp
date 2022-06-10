#include "GraphicsDevice.h"

bool GraphicsDevice::CreateDevice(HWND Window)
{
	//SwapChainDesc

	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

    SwapChainDesc.BufferCount = 3;
    SwapChainDesc.BufferDesc.Width = 2560;
    SwapChainDesc.BufferDesc.Height = 1440;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 80;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = Window;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL featureLevel;


    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, SupportedFeatureLevels, 2, D3D11_SDK_VERSION, &SwapChainDesc, &SwapChain, &Device, &featureLevel, &DeviceContext) != S_OK)
    {
        return false;
    }

    CreateRenderTarget();
    CreateDepthStencil();
    CreateViewport();

    DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

bool GraphicsDevice::CleanupDevice()
{
    CleanupRenderTarget();

    if (SwapChain)
    {
        SwapChain->Release();
    }

    if (DeviceContext)
    {
        DeviceContext->Release();
    }

    if (Device)
    {
        Device->Release();
    }

    return true;
}

void GraphicsDevice::CreateRenderTarget()
{
    //RenderTargetTexture
    SwapChain->GetBuffer(0, IID_PPV_ARGS(&RenderTargetTexture));
    Device->CreateRenderTargetView(RenderTargetTexture.Get(), NULL, &MainRenderTargetView);
}

void GraphicsDevice::CleanupRenderTarget()
{
    if (MainRenderTargetView)
    {
        MainRenderTargetView->Release();
    }
}

void GraphicsDevice::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC depthStencilDesc = {};
    depthStencilDesc.Width = 2560;
    depthStencilDesc.Height = 1440;
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;

    ID3D11Texture2D* depthBufferTexture;
    Device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
    Device->CreateDepthStencilView(depthBufferTexture, 0, &MainDepthStencilView);
    depthBufferTexture->Release();

    DeviceContext->OMSetRenderTargets(1, &MainRenderTargetView, MainDepthStencilView);
}

void GraphicsDevice::CreateViewport()
{
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float) 2560;
    viewport.Height = (float) 1440;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    DeviceContext->RSSetViewports(1, &viewport);
}
