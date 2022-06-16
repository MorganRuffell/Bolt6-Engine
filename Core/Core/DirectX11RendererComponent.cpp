#include "DirectX11RendererComponent.h"
#include <assert.h>
#include <Core/Core/Vertex.h>
#include <thread>


DirectX11RendererComponent::DirectX11RendererComponent(GraphicsDevice* Accel)
	: BaseRendererComponent()
{
	assert(Accel != nullptr);

	m_Accel = Accel->m_Accelerator.get();
	m_GraphicsDevice = Accel;

	InitalizeRasterizerStates(m_Accel);
	ViewportCamera = nullptr;

	InitalizeComponent();

}

void DirectX11RendererComponent::InitalizeComponent()
{
	InitalizeCamera();

	//As we are only using vertex and pixel (fragment) shaders - we are using TRIANGLELIST, in other versions
	//of DirectX we can use patches when working with Hull and Domain shaders

	m_Accel->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	const float colour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	m_Accel->GetDeviceContext()->ClearRenderTargetView(m_GraphicsDevice->GetMainRenderTargetView(), colour);
	m_Accel->GetDeviceContext()->ClearDepthStencilView(m_GraphicsDevice->GetMainDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

	UpdateVertexShaders(StaticMeshVertexShaders, world, Camera, DynamicMeshVertexShaders);
	UpdatePixelShaders(StaticMeshPixelShaders, StaticMeshMaterials, Camera, DynamicMeshPixelShaders, DynamicMeshMaterials);
	
	//Will only be one for now
	std::string NameOfdynamicMeshToUpdate = "Name";

	UpdateDynamicMesh(world->GetDynamicMesh(NameOfdynamicMeshToUpdate),world->GetViewportCamera(), accel, world);
	DrawIndividualDynamicMesh(world->GetDynamicMesh(NameOfdynamicMeshToUpdate),m_Accel);
}

void DirectX11RendererComponent::UpdatePixelShaders(std::vector<PixelShader*>& StaticMeshPixelShaders, std::vector<Material*>& StaticMeshMaterials, BaseCamera* Camera, std::vector<PixelShader*>& DynamicMeshPixelShaders, std::vector<Material*>& DynamicMeshMaterials)
{
	//This updates the resource views of the textures, as well as the sampler states.
	for (int i = 0; i < StaticMeshPixelShaders.size(); i++)
	{
		StaticMeshPixelShaders[i]->SetSamplerState("Sampler", StaticMeshMaterials[0]->GetSamplerState()->GetSampleState());

		//In future if I want to expand the materials, then they go in here, in case they are null, then it's totally fine
		StaticMeshPixelShaders[i]->SetSRV("BaseColour", StaticMeshMaterials[0]->GetDiffuseTexture()->GetResourceView());
		StaticMeshPixelShaders[i]->SetSRV("Normal", StaticMeshMaterials[0]->GetNormalTexture()->GetResourceView());

		StaticMeshPixelShaders[i]->SetFloat3("ViewportCameraPosition", Camera->GetPosition());

		StaticMeshPixelShaders[i]->SetShaderAndCBs();
	}

	for (int i = 0; i < DynamicMeshPixelShaders.size(); i++)
	{
		DynamicMeshPixelShaders[i]->SetSamplerState("Sampler", DynamicMeshMaterials[0]->GetSamplerState()->GetSampleState());

		//In future if I want to expand the materials, then they go in here, in case they are null, then it's totally fine
		DynamicMeshPixelShaders[i]->SetSRV("BaseColour", DynamicMeshMaterials[0]->GetDiffuseTexture()->GetResourceView());
		DynamicMeshPixelShaders[i]->SetSRV("Normal", DynamicMeshMaterials[0]->GetNormalTexture()->GetResourceView());

		DynamicMeshPixelShaders[i]->SetFloat3("ViewportCameraPosition", Camera->GetPosition());

		DynamicMeshPixelShaders[i]->SetShaderAndCBs();
	}
}

void DirectX11RendererComponent::UpdateVertexShaders(std::vector<VertexShader*>& StaticMeshVertexShaders, World* world, BaseCamera* Camera, std::vector<VertexShader*>& DynamicMeshVertexShaders)
{
	for (auto& element : StaticMeshVertexShaders)
	{
		element->SetMatrix4x4("world", world->GetWorldMatrix());
		element->SetMatrix4x4("view", Camera->GetViewMatrix());
		element->SetMatrix4x4("projection", Camera->GetProjectionMatrix());
		element->SetShaderAndCBs();
	}

	for (auto& element : DynamicMeshVertexShaders)
	{
		element->SetMatrix4x4("world", world->GetWorldMatrix());
		element->SetMatrix4x4("view", Camera->GetViewMatrix());
		element->SetMatrix4x4("projection", Camera->GetProjectionMatrix());
		element->SetShaderAndCBs();
	}
}

void DirectX11RendererComponent::UpdateDynamicMesh(DynamicMesh* Mesh, BaseCamera* Camera, Accelerator* Accel, World* world)
{
	//Get the skeleton from the mesh, and cache all of the mesh elements
	Skeleton* Skeleton = Mesh->GetSkeleton();

	std::vector<Material*>			Materials;
	std::vector<PixelShader*>		PixelShaders;
	std::vector<VertexShader*>		VertexShaders;

	int DynamicMeshBoneCount = Mesh->GetSkeleton()->mBones.size();

	//Appen all of the materials to these arrays ready for rendering.
	for (int i = 0; i < Mesh->MeshMaterials.size(); i++)
	{
		Materials.push_back(Mesh->GetMaterialatIndex(i));
	}

	for (auto& element : Materials)
	{
		PixelShaders.push_back(element->m_MaterialPixelShader);
		VertexShaders.push_back(element->m_MaterialVertexShader);
	}

	for (auto& Shader : VertexShaders )
	{
		Shader->SetMatrix4x4("world", world->GetWorldMatrix());
		Shader->SetMatrix4x4("view", world->GetViewportCamera()->GetViewMatrix());
		Shader->SetMatrix4x4("projection", world->GetViewportCamera()->GetProjectionMatrix());
	}

	DynamicMeshBoneCount = sizeof(XMFLOAT4X4);
	DynamicMeshBoneCount *= 100; //Multiply by maximum amount of bones rendering
	DynamicMeshBoneCount * 2 ; //Create a small buffer, just in case.

	Bone2 bones[100];

	UpdateBoneData(Mesh, Skeleton, bones);

	for (auto& Shader : VertexShaders)
	{
		Shader->SetData("Bones", &bones, DynamicMeshBoneCount);

		auto BlendweightDefault = 1.0f;

		Shader->SetData("blendWeight", &BlendweightDefault, sizeof(float));
	}

	for (auto& Shader : PixelShaders)
	{
		for (auto& Material : Materials)
		{
			Shader->SetSamplerState("Sampler", Material->GetSamplerState()->GetSampleState());
		
			Shader->SetShaderResourceView("DiffuseTexture", Material->GetDiffuseTexture()->GetResourceView());
			Shader->SetShaderResourceView("NormalTexture", Material->GetNormalTexture()->GetResourceView());
		}
	}

	UpdateShaders(VertexShaders, PixelShaders);

	auto VertexBuffer = Mesh->GetVertexBuffer();
	UINT stride = sizeof(Vertex3); //stride is the size of each vertex
	UINT offset = 0; //Offset in buffers, we've tightly packed these buffers

	auto AcceleratorContext = Accel->GetDeviceContext();

	AcceleratorContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	AcceleratorContext->IASetIndexBuffer(Mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	AcceleratorContext->DrawIndexed(Mesh->GetIndexCount(), 0, 0);
}

void DirectX11RendererComponent::UpdateBoneData(DynamicMesh* Mesh, Skeleton* Skeleton, Bone2  bones[100])
{
	int NumberOfBones = Mesh->GetSkeleton()->mBones.size();

	for (int i = 0; i < NumberOfBones; i++)
	{
		XMMATRIX jointTransformMatrix = XMLoadFloat4x4(&Skeleton->mBones[i]->BoneTransform);
		XMMATRIX invJointTransformMatrix = XMLoadFloat4x4(&Skeleton->mBones[i]->mRelativeBindposeInverse);

		XMFLOAT4X4 trans = {};
		XMStoreFloat4x4(&trans, XMMatrixTranspose(jointTransformMatrix));
		bones[i].BoneTransform = trans;

		XMFLOAT4X4 trans2 = {};
		XMStoreFloat4x4(&trans2, XMMatrixTranspose(invJointTransformMatrix));
		bones[i].InvBoneTransform = trans2;
	}
}

void DirectX11RendererComponent::UpdateShaders(std::vector<VertexShader*>& VertexShaders, std::vector<PixelShader*>& PixelShaders)
{
	for (auto& Shader : VertexShaders)
	{
		Shader->CopyAllBufferData();
		Shader->SetShaderAndCBs();
	}

	for (auto& Shader : PixelShaders)
	{
		Shader->CopyAllBufferData();
		Shader->SetShaderAndCBs();
	}
}

//Swaps the swapchain, these are where the render targets are swapped round.
void DirectX11RendererComponent::Render(GraphicsDevice* accel)
{
	accel->GetSwapChain()->Present(1, 0);
}

void DirectX11RendererComponent::Render(GraphicsDevice* accel, int VSyncOpt1, int VSyncOpt2)
{
	accel->GetSwapChain()->Present(VSyncOpt1, VSyncOpt2);
}


