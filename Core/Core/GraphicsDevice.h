#pragma once

#include "GraphicsDeviceBase.h"
#include <wrl.h>


using namespace Microsoft::WRL;

class GraphicsDevice : public GraphicsDeviceBase
{
public:

	bool CreateDevice(HWND Window) override;

	bool CleanupDevice() override;

	void CreateRenderTarget() override;

	void CleanupRenderTarget() override;

public:

	void CreateDepthStencil();
	void CreateViewport();


public:

	ID3D11Device* GetDevice()
	{
		return Device.Get();
	}

	ID3D11DeviceContext* GetDeviceContext()
	{
		return DeviceContext.Get();
	}

public:

	IDXGISwapChain* GetSwapChain()
	{
		return SwapChain.Get();
	}

	ID3D11RenderTargetView* GetMainRenderTargetView()
	{
		return MainRenderTargetView;
	}

	const ID3D11RenderTargetView* GetConstMainRenderTargetView()
	{
		return MainRenderTargetView;
	}

private:

	ComPtr<ID3D11Device>			Device;
	ComPtr<ID3D11DeviceContext>		DeviceContext;

public:

	ID3D11RenderTargetView*	MainRenderTargetView;
	ID3D11DepthStencilView* MainDepthStencilView;

private:

	ComPtr<ID3D11Texture2D>			RenderTargetTexture;

};

