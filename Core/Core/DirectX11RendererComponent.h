#pragma once

#include "BaseRendererComponent.h"
#include "RasterizerState.h"
#include <memory>
#include <Core/Core/GraphicsDevice.h>
#include "BaseCamera.h"
#include <mutex>

class DirectX11RendererComponent : public BaseRendererComponent
{
public:

	DirectX11RendererComponent(GraphicsDevice* Accel);


	virtual void InitalizeComponent() override;
	virtual bool TerminateComponent() override;

	virtual void Update(World* world, BaseCamera* Camera, Accelerator* accel) override;

	void UpdatePixelShaders(std::vector<PixelShader*>& StaticMeshPixelShaders, std::vector<Material*>& StaticMeshMaterials, BaseCamera* Camera, std::vector<PixelShader*>& DynamicMeshPixelShaders, std::vector<Material*>& DynamicMeshMaterials);
	void UpdateVertexShaders(std::vector<VertexShader*>& StaticMeshVertexShaders, World* world, BaseCamera* Camera, std::vector<VertexShader*>& DynamicMeshVertexShaders);

	void UpdateStaticMesh(StaticMesh* Mesh, BaseCamera* Camera, Accelerator* Accel, World* world);
	void UpdateDynamicMesh(DynamicMesh* Mesh, BaseCamera* Camera, Accelerator* Accel, World* world);
	void UpdateBoneData(DynamicMesh* Mesh, Skeleton* Skeleton, Bone2  bones[100]);

	void UpdateShaders(std::vector<VertexShader*>& VertexShaders, std::vector<PixelShader*>& PixelShaders);


	void Render(GraphicsDevice* accel, int VSyncOpt1, int VSyncOpt2) override;

protected:

	void InitalizeRasterizerStates(Accelerator* Accel);
	void DestroyRasterizerStates();

	bool InitalizeCamera();

public:

	BaseCamera* GetViewportCamera()
	{
		return ViewportCamera.get();
	}


public:

	void BeginFrame(Accelerator* Accel);
	void EndFrame(Accelerator* Accel);

protected:

	void DrawIndividualStaticMesh(StaticMesh* Mesh, Accelerator* Accel);
	void DrawIndividualDynamicMesh(DynamicMesh* Mesh, Accelerator* Accel);

protected:

	RasterizerState*	StandardRS;
	RasterizerState*	WireframeRS;

	Accelerator*		m_Accel;
	GraphicsDevice*		m_GraphicsDevice;
protected:

	std::unique_ptr<BaseCamera>			ViewportCamera;


	int AmountOfRasterizerStates;

protected:

	ID3D11BlendState*				m_BlendState;
	ID3D11DepthStencilState*		m_DepthStencilState;

protected:

	D3D11_PRIMITIVE_TOPOLOGY		primitiveTopology;


	// Inherited via BaseRendererComponent
	virtual void Render(GraphicsDevice* accel) override;

};

