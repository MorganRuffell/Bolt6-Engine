#pragma once

#include <DirectXMath.h>
//#include "Vertex.h"
#include <d3d11.h>
#include <vector>
#include <Core/Core/Material.h>
#include <Core/Core/Object.h>

class BaseMesh : public Object
{

public:

	void CreateMaterial(Accelerator* _Accel, std::string& Name)
	{
		TESamplerState* SamplerState = new TESamplerState(_Accel);
		Material* _Matty = new Material(SamplerState, Name, _Accel);
		MeshMaterials.push_back(_Matty);

	}

	void CreateMaterial(Accelerator* _Accel)
	{
		TESamplerState* SamplerState = new TESamplerState(_Accel);

		std::string Name = "Unammed Material";

		Material* _Matty = new Material(SamplerState, Name, _Accel);

		MeshMaterials.push_back(_Matty);
	}

public:

	void CreateMaterial(Accelerator* _Accel, std::string& Name, TextureContext DiffuseContext)
	{
		Texture* DiffuseText = new Texture(DiffuseContext, Name, _Accel);

		TESamplerState* SamplerState = new TESamplerState(_Accel);
		Material* _Matty = new Material(SamplerState, Name, _Accel);

		MeshMaterials.push_back(_Matty);
	}

	void CreateMaterial(Accelerator* _Accel, TextureContext DiffuseContext)
	{
		TESamplerState* SamplerState = new TESamplerState(_Accel);

		std::string Name = "Unammed Material";

		Texture* DiffuseText = new Texture(DiffuseContext, Name, _Accel);

		Material* _Matty = new Material(SamplerState, Name, _Accel);
		MeshMaterials.push_back(_Matty);
	}

	void CreateMaterial(Accelerator* _Accel, std::string& Name, TextureContext DiffuseContext, TextureContext NormalContext)
	{
		Texture* DiffuseText = new Texture(DiffuseContext, Name,_Accel);
		Texture* NormalText = new Texture(NormalContext, Name, _Accel);

		TESamplerState* SamplerState = new TESamplerState(_Accel);
		Material* _Matty = new Material(SamplerState, Name, _Accel);

		MeshMaterials.push_back(_Matty);
	}

	void CreateMaterial(Accelerator* _Accel, TextureContext DiffuseContext, TextureContext NormalContext)
	{
		TESamplerState* SamplerState = new TESamplerState(_Accel);

		std::string Name = "Unammed Material";

		Texture* DiffuseText = new Texture(DiffuseContext, Name, _Accel);
		Texture* NormalText = new Texture(NormalContext, Name, _Accel);

		Material* _Matty = new Material(SamplerState, Name, _Accel);
		MeshMaterials.push_back(_Matty);
	}

public:

	ID3D11Buffer* GetVertexBuffer()
	{
		return VertexBuffer;
	}

	ID3D11Buffer* GetIndexBuffer()
	{
		return IndexBuffer;
	}

public:

	Material* GetMaterialatIndex(int index)
	{
		if (index > MeshMaterials.size())
		{
			return nullptr;
		}
		else
		{
			return MeshMaterials.at(index);

		}
	}


	int GetIndexCount()
	{
		return IndexCount;
	}



public:

	std::vector<Material*> MeshMaterials;

protected:

	int				IndexCount;
	int				m_Indicies;

	int				TrisCount;
	int				EdgesCount;

protected:

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	D3D11_BUFFER_DESC VertexBufferDesc;
	D3D11_BUFFER_DESC IndexBufferDesc;


};