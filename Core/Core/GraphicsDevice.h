#pragma once

#include "GraphicsDeviceBase.h"
#include <memory>
#include <Core/Core/Engine/Graphics/Accelerator.h>


using namespace Microsoft::WRL;

class GraphicsDevice : public GraphicsDeviceBase
{
public:
	GraphicsDevice()
	{
		SetTag(EngineObjTag::Engine);
	}

public:

	bool CreateDevice(HWND Window) override;

	bool CleanupDevice() override;

	void CreateRenderTarget() override;

	void CleanupRenderTarget() override;

public:

	void CreateDepthStencil();
	void CreateViewport();

	std::shared_ptr<Accelerator> m_Accelerator;

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

	ID3D11DepthStencilView* GetMainDepthStencilView()
	{
		return MainDepthStencilView;
	}

public:

	bool UseVsync;

public:

	ID3D11RenderTargetView*	MainRenderTargetView;
	ID3D11DepthStencilView* MainDepthStencilView;


private:

	ComPtr<ID3D11Texture2D>			RenderTargetTexture;
	
};

