#pragma once
#include <d3d11.h>
#include <Core/Core/Engine/Graphics/Accelerator.h>

class BaseRasterizerState
{
protected:

	virtual void InitalizeState(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, Accelerator* Accel) = 0;
	virtual void ModifyState() = 0;
	virtual void DestroyState() = 0;

protected:

	D3D11_FILL_MODE FillMode;
	D3D11_CULL_MODE CullMode;

protected:

	ID3D11RasterizerState* m_RasterizerState;
	D3D11_RASTERIZER_DESC RasterizerDesc;
};

