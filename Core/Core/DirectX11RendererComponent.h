#pragma once

#include "BaseRendererComponent.h"
#include "RasterizerState.h"
#include <memory>

class DirectX11RendererComponent : public BaseRendererComponent
{
public:

	DirectX11RendererComponent(Accelerator* Accel);

	virtual void InitalizeComponent() override;
	virtual bool TerminateComponent() override;

protected:

	void InitalizeRasterizerStates(Accelerator* Accel);
	void DestroyRasterizerStates();


protected:

	RasterizerState* StandardRS;
	RasterizerState* WireframeRS;


private:

	int AmountOfRasterizerStates;

protected:

	ID3D11BlendState*				m_BlendState;
	ID3D11DepthStencilState*		m_DepthStencilState;

protected:

	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

};

