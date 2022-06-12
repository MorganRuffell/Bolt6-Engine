#pragma once

#include "BaseRendererComponent.h"
#include "RasterizerState.h"
#include <memory>
#include <Core/Core/GraphicsDevice.h>
#include "BaseCamera.h"

class DirectX11RendererComponent : public BaseRendererComponent
{
public:

	DirectX11RendererComponent(GraphicsDevice* Accel);

	virtual void InitalizeComponent() override;
	virtual bool TerminateComponent() override;



protected:

	void InitalizeRasterizerStates(Accelerator* Accel);
	void DestroyRasterizerStates();


protected:

	void BeginFrame(Accelerator* Accel);
	void EndFrame(Accelerator* Accel);


protected:

	RasterizerState* StandardRS;
	RasterizerState* WireframeRS;


protected:

	BaseCamera*			ViewportCamera;


	int AmountOfRasterizerStates;

protected:

	ID3D11BlendState*				m_BlendState;
	ID3D11DepthStencilState*		m_DepthStencilState;

protected:

	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;

	// Inherited via BaseRendererComponent
	virtual void Update() override;
	virtual void Render() override;
};

