#pragma once

/*
	Graphics Device base, this contains all of the DirectX components that must be initalized
	This is specialized in to GraphicsDevice, with all the unique 11 series components.

*/

#include <d3d11.h>
#include <wrl.h>
#include <cassert>

using namespace Microsoft::WRL;


class GraphicsDeviceBase
{
protected:

	virtual bool CreateDevice(HWND Window) = 0;
	virtual bool CleanupDevice() = 0;

protected:

	virtual void CreateRenderTarget() = 0;
	virtual void CleanupRenderTarget() = 0;

protected:

	ComPtr<IDXGISwapChain> SwapChain;
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;

	const D3D_FEATURE_LEVEL SupportedFeatureLevels[3] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};

};

