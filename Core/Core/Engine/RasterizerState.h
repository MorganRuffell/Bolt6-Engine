#pragma once

#include "BaseRasterizerState.h"

class RasterizerState : public BaseRasterizerState
{
public:
	RasterizerState()
	{

	}

	virtual void InitalizeState(D3D11_FILL_MODE FillMode, D3D11_CULL_MODE CullMode, Accelerator* Accel) override;

	virtual void ModifyState() override;

	virtual void DestroyState() override;

};

