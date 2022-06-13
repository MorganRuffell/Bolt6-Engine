#include "RasterizerState.h"
#include <assert.h>
#include <iostream>

void RasterizerState::InitalizeState(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, Accelerator* Accel)
{
	assert(Accel != nullptr);

	RasterizerDesc = {};

	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	RasterizerDesc.FillMode = FillMode;
	RasterizerDesc.CullMode = CullMode;
	RasterizerDesc.FrontCounterClockwise = false;
	RasterizerDesc.DepthClipEnable = true;
	RasterizerDesc.AntialiasedLineEnable = true;

	HRESULT RasteriserRes = Accel->Device->CreateRasterizerState(&RasterizerDesc, &m_RasterizerState);

	if (RasteriserRes != S_OK)
	{
		std::cout << "Failed to initalize Rasterizer state!" << std::endl;
	}

	std::cout << "Initalized Rasterizer State!" << std::endl;

	//Accel->DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); -- To be done in main renderer.
}

void RasterizerState::ModifyState()
{
}

void RasterizerState::DestroyState()
{
	ZeroMemory(&RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	if (m_RasterizerState != nullptr)
	{
		m_RasterizerState->Release();
	}
}
