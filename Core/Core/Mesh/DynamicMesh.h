#pragma once

#include <Core/Core/Mesh/BaseMesh.h>
#include <Core/Core/FBX/SkeletalMeshComponents/Skeleton.h>
#include <Core/Core/Engine/Animation/AnimationStructures.h>
#include <Core/Core/Vertex.h>


class DynamicMesh : public BaseMesh
{
public:

	DynamicMesh(Skeleton* _skl, AnimationSequence* _Seq, Vertex3* Vertexes, int vertexCount, int* Indicies, int indexCount, Accelerator* _Accel)
	{
		SetTag(EngineObjTag::Asset);
		m_skeleton = _skl;
		m_Animation = _Seq;

		m_Indicies = vertexCount / 2;
		IndexCount = indexCount;

		TrisCount = vertexCount / 3;


		VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		VertexBufferDesc.ByteWidth = sizeof(Vertexes) * vertexCount;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = 0;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initialVertexData = {};
		initialVertexData.pSysMem = Vertexes;

		HRESULT res = _Accel->GetDevice()->CreateBuffer(&VertexBufferDesc, &initialVertexData, &VertexBuffer);

		if (!SUCCEEDED(res))
		{
			std::cout << "Vertex Buffer Creation failed!" << std::endl;
			return;
		}

		IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		IndexBufferDesc.ByteWidth = sizeof(int) * indexCount;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = 0;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initalIndexData = {};
		initalIndexData.pSysMem = &Indicies; //??

		res = _Accel->GetDevice()->CreateBuffer(&IndexBufferDesc, &initalIndexData, &IndexBuffer);

		if (!SUCCEEDED(res))
		{
			std::cout << "Index Buffer Creation failed!" << std::endl;
			return;
		}


	}

	DynamicMesh(Skeleton* _skl, Vertex3* Vertexes, int vertexCount, int* Indicies, int indexCount, Accelerator* _Accel)
	{
		SetTag(EngineObjTag::Asset);
		m_skeleton = _skl;

		m_Indicies = vertexCount / 2;
		IndexCount = indexCount;

		TrisCount = vertexCount / 3;


		VertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		VertexBufferDesc.ByteWidth = sizeof(Vertexes) * vertexCount;
		VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		VertexBufferDesc.MiscFlags = 0;
		VertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initialVertexData = {};
		initialVertexData.pSysMem = Vertexes;
		initialVertexData.SysMemPitch = 0;
		initialVertexData.SysMemSlicePitch = 0;


		HRESULT res = _Accel->GetDevice()->CreateBuffer(&VertexBufferDesc, &initialVertexData, &VertexBuffer);

		if (!SUCCEEDED(res))
		{
			std::cout << "Vertex Buffer Creation failed!" << std::endl;
			return;
		}

		IndexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		IndexBufferDesc.ByteWidth = sizeof(int) * indexCount;
		IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		IndexBufferDesc.MiscFlags = 0;
		IndexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initalIndexData = {};
		initalIndexData.pSysMem = &Indicies; //??

		res = _Accel->GetDevice()->CreateBuffer(&IndexBufferDesc, &initalIndexData, &IndexBuffer);

		if (!SUCCEEDED(res))
		{
			std::cout << "Index Buffer Creation failed!" << std::endl;
			return;
		}
	}

	~DynamicMesh()
	{

	}

public:

	Skeleton* GetSkeleton()
	{
		return m_skeleton;
	}

	void SetSkeleton(Skeleton* Skl)
	{
		m_skeleton = Skl;
	}

public:

	AnimationSequence* GetSequence()
	{
		return m_Animation;
	}


	void CalculateTangents(Vertex3* vertices, int vertexCount, int* indices, int indexCount);


protected:

	Skeleton*					m_skeleton;
	AnimationSequence*			m_Animation;
};

