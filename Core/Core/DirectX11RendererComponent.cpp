#include "DirectX11RendererComponent.h"
#include <assert.h>
#include <Core/Core/Vertex.h>
#include <thread>



DirectX11RendererComponent::DirectX11RendererComponent(GraphicsDevice* Accel)
	: BaseRendererComponent()
{
	assert(Accel != nullptr);

	InitalizeRasterizerStates(Accel->m_Accelerator.get());
	ViewportCamera = nullptr;

	InitalizeComponent();

}

void DirectX11RendererComponent::InitalizeComponent()
{
	InitalizeCamera();
}

bool DirectX11RendererComponent::TerminateComponent()
{
	return false;
}

void DirectX11RendererComponent::InitalizeRasterizerStates(Accelerator* Accel)
{
	StandardRS = new RasterizerState();
	WireframeRS = new RasterizerState();

	StandardRS->InitalizeState(D3D11_FILL_SOLID, D3D11_CULL_BACK, Accel);
	WireframeRS->InitalizeState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE, Accel);
}

void DirectX11RendererComponent::DestroyRasterizerStates()
{
	delete StandardRS;
	delete WireframeRS;
}

bool DirectX11RendererComponent::InitalizeCamera()
{
	ViewportCamera = std::make_unique<BaseCamera>();

	if (ViewportCamera != nullptr)
	{
		std::cout << "Viewport Camera initalized!" << std::endl;
		return true;
	}
	else
	{
		return false;
	}

}

void DirectX11RendererComponent::BeginFrame(Accelerator* Accel)
{

}

void DirectX11RendererComponent::EndFrame(Accelerator* Accel)
{
}

void DirectX11RendererComponent::DrawIndividualStaticMesh(StaticMesh* Mesh, Accelerator* Accel)
{
	assert(Mesh != nullptr);
	assert(Accel->DeviceContext != nullptr);

	UINT Stride = sizeof(Vertex3);
	UINT Offset = 0;

	auto VertexBuffer = Mesh->GetVertexBuffer();
	auto IndexBuffer = Mesh->GetIndexBuffer();

	Accel->DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	Accel->DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Accel->DeviceContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);

}

void DirectX11RendererComponent::DrawIndividualDynamicMesh(DynamicMesh* Mesh, Accelerator* Accel)
{
	assert(Mesh != nullptr);
	assert(Accel != nullptr);

	UINT Stride = sizeof(Vertex3);
	UINT Offset = 0;

	auto VertexBuffer = Mesh->GetVertexBuffer();
	auto IndexBuffer = Mesh->GetIndexBuffer();

	Accel->DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &Stride, &Offset);
	Accel->DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Accel->DeviceContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);

}

//Render is where we present the swapchain with it's paramaters. Update is where we set everything up that we need.
void DirectX11RendererComponent::Update(World* world, BaseCamera* Camera, Accelerator* accel)
{
	assert(world != nullptr && Camera != nullptr && accel != nullptr);
	assert(accel->DeviceContext != nullptr);

	UINT Stride = sizeof(Vertex3);
	UINT Offset = 0;

	std::vector<Material*> StaticMeshMaterials;
	std::vector<Material*> DynamicMeshMaterials;

	std::vector<VertexShader*> StaticMeshVertexShaders;
	std::vector<VertexShader*> DynamicMeshVertexShaders;

	std::vector<PixelShader*> StaticMeshPixelShaders;
	std::vector<PixelShader*> DynamicMeshPixelShaders;


	for (auto& element : world->GetDynamicMeshes())
	{
		for (int i = 0; i < element->MeshMaterials.size(); i++)
		{
			DynamicMeshPixelShaders.push_back(element->GetMaterialatIndex(0)->m_MaterialPixelShader);
			DynamicMeshVertexShaders.push_back(element->GetMaterialatIndex(0)->m_MaterialVertexShader);
			DynamicMeshMaterials.push_back(element->GetMaterialatIndex(0));
		}
	}


	for (auto& element : world->GetStaticMeshes())
	{
		for (int i = 0; i < element->MeshMaterials.size(); i++)
		{
			StaticMeshPixelShaders.push_back(element->GetMaterialatIndex(0)->m_MaterialPixelShader);
			StaticMeshVertexShaders.push_back(element->GetMaterialatIndex(0)->m_MaterialVertexShader);
			StaticMeshMaterials.push_back(element->GetMaterialatIndex(0));
		}
	}

	for (auto& element : StaticMeshVertexShaders)
	{
		element->SetMatrix4x4("world", world->GetWorldMatrix());
		element->SetMatrix4x4("view", Camera->GetViewMatrix());
		element->SetMatrix4x4("projection", Camera->GetProjectionMatrix());
	}

	for (auto& element : DynamicMeshVertexShaders)
	{
		element->SetMatrix4x4("world", world->GetWorldMatrix());
		element->SetMatrix4x4("view", Camera->GetViewMatrix());
		element->SetMatrix4x4("projection", Camera->GetProjectionMatrix());
	}

	//Finish import procedure before going further!
}

//Swaps the swapchain.
void DirectX11RendererComponent::Render(GraphicsDevice* accel)
{
	accel->GetSwapChain()->Present(1, 0);
}

void DirectX11RendererComponent::Render(GraphicsDevice* accel, int VSyncOpt1, int VSyncOpt2)
{
	accel->GetSwapChain()->Present(VSyncOpt1, VSyncOpt2);
}


