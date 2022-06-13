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

	virtual void Update(World* world, BaseCamera* Camera, Accelerator* accel) override;

	void Render(GraphicsDevice* accel, int VSyncOpt1, int VSyncOpt2) override;

protected:

	void InitalizeRasterizerStates(Accelerator* Accel);
	void DestroyRasterizerStates();

	bool InitalizeCamera();


protected:

	void BeginFrame(Accelerator* Accel);
	void EndFrame(Accelerator* Accel);


	void DrawIndividualStaticMesh(StaticMesh* Mesh, Accelerator* Accel);
	void DrawIndividualDynamicMesh(DynamicMesh* Mesh, Accelerator* Accel);

protected:

	RasterizerState* StandardRS;
	RasterizerState* WireframeRS;


protected:

	std::unique_ptr<BaseCamera>			ViewportCamera;


	int AmountOfRasterizerStates;

protected:

	ID3D11BlendState*				m_BlendState;
	ID3D11DepthStencilState*		m_DepthStencilState;

protected:

	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;


	

	// Inherited via BaseRendererComponent
	virtual void Render(GraphicsDevice* accel) override;

};

